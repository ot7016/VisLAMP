#include "ReadData.hpp"

using namespace std;

/****
最初のinputでnum dim atrの大きさを読み込んで決める
num 項目の数
atr 属性数
dim 高次元配置の次元数 

****/

ReadData::ReadData(){
	readfname();
  read(dataid);
}
ReadData::~ReadData(){
  delete[] A;
  delete[] D;
  delete[] evector;
  delete[] order;
  delete[] evalue;
  }


void ReadData::readfname(){
  unsigned char isFolder =0x4;
  //DIR* Dir;
  struct dirent *DirEntry;
  DIR* dir = opendir("../data/");
  while(DirEntry =readdir(dir)){
   //cout << DirEntry->d_name;
   if ( DirEntry->d_type == isFolder)
   {
    char* folder = DirEntry->d_name;
    if(folder[0] != '.'){
  cout <<"Found a Directory : " <<folder << endl;
  dataname.push_back(folder);
}
   }
}
  closedir(dir);
}

void ReadData::clearall(){
  delete[] A;
  delete[] D;
  delete[] evector;
  delete[] order;
  delete[] evalue;
  name.clear();
  name.shrink_to_fit();
  atrname.clear();
  atrname.shrink_to_fit();
  cluster.clear();
  cluster.shrink_to_fit();
  filterindex.clear();
  edge.clear();
  edge.shrink_to_fit();
  filteredge.clear(); 
  filteredge.shrink_to_fit(); 
  notselectedindex.clear();
}
void ReadData::read(int id){
  dataid = id;
   string d = dataname.at(id);
  string dir = "../data/" + d +"/"+ d;
  readsetting(dir);
  readcood(dir);
  readevalue(dir);
  readDist(dir);
  readoriginal(dir);
}

void ReadData::readsetting(string dir){
  ifstream ifs(dir+ "-input.txt");
  if (ifs.fail()){
        cerr << "失敗" << endl;
        exit(1);
    }
    string str;
  
  getline(ifs,str);
  thr = stod(str);
  pthr = thr;
  getline(ifs,str);
  num = stoi(str);
  getline(ifs,str);
  dim = stoi(str);
  getline(ifs,str);
  atr = stoi(str);
  getline(ifs,str);
  if(str == "PCA"){
    isPCA =true;
    aginum = num;
  }
  else if(str == "MDS"){
    isPCA =false;
    aginum = num+atr;
  } 
  
  isTSP = false;
  isLenVar = true;
  isCoord =false;
  order = new int[atr];
  for(int i = 0;i<atr;i++){
    order[i] = i;
  }
  lastclickid = -1;
}

void ReadData::readcood(string dir){	
	//ここでAを動的確保
  //論文中ではAを元のデータ行列としてしまっているがこのプログラム中では高次元配置
	//Aの読み取り 
	fstream fs(dir+"-cood.dat",ios::in | ios::binary);
	if (!fs){
        cerr << "cood失敗" << endl;
        exit(1);
    }
    // A は (num+atr) * dimの行列 
    A = new double[aginum * dim]; 
    fs.seekg(0);
    fs.read((char*) A, sizeof (double) * aginum  * dim);

}
void ReadData::readevalue(string dir){
  fstream fs2(dir+"-evalue.dat",ios::in | ios::binary);
	if (!fs2){
    cerr << "evalue失敗" << endl;
     exit(1);
  }  

  evalue = new double[dim];
  fs2.seekg(0);
  fs2.read((char*) evalue, sizeof (double)* dim);
  
  if(isPCA){
    fstream fs3(dir+"-evector.dat",ios::in | ios::binary);
    if (!fs2){
      cerr << "evector失敗" << endl;
      exit(1);
    }
    //evectorも dim (=atr) * dimの1次元配列 
    evector = new double[dim * dim];
    fs3.seekg(0);
    fs3.read((char*) evector, sizeof (double)* dim *dim);
  }


}


void ReadData::readoriginal(string dir){

//string str1[atr];
  //array<string, atr> atrname;  // = new string[atr];
  string str;
  ifstream ifs1 (dir+ "-atrname.csv");
  if (ifs1.fail()) {
        cerr << "File Does not exist" << endl;
        exit(1);
    }
    getline(ifs1,str);
  while(getline(ifs1,str)){
    atrname.push_back(str);
}
    ifstream ifs2 (dir+ "-name.csv");
  if (ifs2.fail()) {
        cerr << "File Does not exist" << endl;
        exit(1);
    }
    getline(ifs2,str);
 
   while(getline(ifs2,str)){
    name.push_back( str);
   }
   

  fstream fs(dir+"-original.dat");
  if(!fs){
    cerr << "original失敗" << endl;
        exit(1);
  }
  double nums[num][atr];
  fs.seekg(0);
  fs.read((char*) nums, sizeof (double) * num * atr);
  D = new double*[num];
  for(int i = 0; i< num;i++){
     D[i] = new double[atr];
    for(int j = 0;j<atr;j++){
      D[i][j] = nums[i][j];
    }
  }
  Dmin = new double[atr];
  Dmax = new double[atr];
  for(int i = 0; i< atr; i++){
    Dmin[i] = D[0][i];
    Dmax[i] = D[0][i];   
  }

  for(int i = 0; i< num; i++){
    for(int j = 0; j<atr; j++){
      double d = D[i][j];
      if(Dmin[j] >d)
        Dmin[j] = d;
      if(Dmax[j] < d)
        Dmax[j] = d; 
    }
  }
   for(int i = 0;i<num;i++){
      filterindex.push_back(i);
      notselectedindex.push_back(i);
  }
}
void ReadData::readDist(string dir){
  ifstream ifs;
  int src_size;
  int status = STATUS_OK;
  ifs.open(dir+"-dist.dat",ios::binary);
  src_size = ifs.tellg();
  string str;
  if (ifs.fail()){
        cerr << "dist失敗" << endl;
        exit(1);
  }
    
  // alldistを全て読み込まないようにする 
  double *dist;
  int len = DIST_SIZE;
  ifs.seekg(0);
  alldist = new double[aginum * aginum];
  dist = new double[len]; 
  int r = 0;
  while( (status == STATUS_OK) && (!ifs.eof())) {
    ifs.read((char*) dist, sizeof(double) * len);
    int lim = pow(aginum,2) -len *r;
    int to = min(len, lim);
    for(int k = 0; k < to;k++){
      int i = (k + r*len)/ aginum;
      int j =( k + r*len) % aginum;
     alldist[k +r * len] = dist[k];
      if(i < j && dist[k] <  thr){
        edge.push_back(pair<int, int>(i,j));
       filteredge.push_back(pair<int,int>(i,j));
     }
    }
    r++;
  }
  ifs.close();
  delete[] dist;
  //alldistの最大値を求めておく
  distmax = 0;
  for(int i = 0; i< num; i++){
      for(int j = i+1; j< num;j++){
          double d =　alldist[i*num +j] ;
           if(distmax < d)
            distmax = d; 
      }
  }
}
//一応残しておく
void ReadData::calEdge(){
  for(int i = 0; i < aginum;i++){
    for(int j = i+1; j < aginum; j++){
      double s = 0;
      for(int k = 0; k < dim; k++ ){
        s = s + pow(getA(i,k) - getA(j,k) ,2);
      }
       double e = sqrt(s);
      if(e < thr)
      edge.push_back(pair<int,int>(i,j));
    }
    
  }
}
void ReadData::recalEdge(double t){
  edge.clear();
  filteredge.clear();
  thr = t;
  for(int k = 0; k < aginum*aginum; k++){
    int i = k / aginum;
    int j = k % aginum;
    if(i< j && alldist[k] < thr){
      edge.push_back(pair<int,int>(i,j));
      filteredge.push_back(pair<int,int>(i,j));
    }
  }
  filterindex.clear();
  for(int i = 0;i < num;i++){
    filterindex.push_back(i);
  }
  repairNIndex();
}

double ReadData::getevector(int i, int j){
  return evector[dim*i + j];
}
// 今は Aだけ1次元配列で実装している
double ReadData::getA(int i, int j){
	return A[i* dim +j];
}
void ReadData::setOrder(TSPsolver* ts){
  order = new int[atr];
  for(int i= 0;i<atr;i++){
    order[i] = ts->getorder(i);
  }
  isCoord = false;
}
void ReadData::setOrder(int* o){
  order = new int[atr];
  for(int i = 0;i<atr;i++){
    order[i] = o[i];
  }
  isCoord =false;
}

vector<std::pair<int, int> > ReadData::getEdge(){
  return edge;
}

void ReadData::turnLenVar(){
  isLenVar= !isLenVar;
}
vector<S_Cluster > ReadData::getCluster(){
  return cluster;
}

list<int>ReadData::getFIndex(){
  return filterindex;
}

list<int> ReadData::getNSIndex(){
  return notselectedindex;
}
//今は前の選択を保持しない形とする
void ReadData::setSIndex(int i){
  list<int> selected;
  selected.push_back(i);
  RGB rgb = setColor();
  cluster.push_back(S_Cluster(selected,rgb));
  notselectedindex.remove(i);
}

void ReadData::setSIndex(int j, vector<double> v, int clickid){
  list<int> selected;
  sort(v.begin(),v.end());
  for(int i = 0;i < num;i++){
    if( D[i][j] > v.at(0) && D[i][j] < v.at(1) )
      selected.push_back(i);
  }

  if(clickid == lastclickid){
    if(cluster.back().index.size() > selected.size()){
      cluster.pop_back();
      repairNIndex();
    }
    else  cluster.pop_back();
  }
    RGB rgb = setColor();
  cluster.push_back(S_Cluster(selected, rgb));
  for(int i : selected){
    notselectedindex.remove(i);     
  }
  lastclickid = clickid;
}

//フィルターを今までのリストとどう扱うかが問題

void ReadData::setSIndex(list<int> v,int clickid){

  if(clickid == lastclickid){
    if(cluster.back().index.size() > v.size()){
       cluster.pop_back();
      repairNIndex();
    }
    else  cluster.pop_back();
  }
    RGB rgb = setColor();
  cluster.push_back(S_Cluster(v, rgb));
 // auto ite ;
    for(int i:v){
      notselectedindex.remove(i);     
    }
  lastclickid = clickid;
}

void ReadData::repairNIndex(){
  notselectedindex.clear();
  for(int i: filterindex){
    notselectedindex.push_back(i);
  }
  for(auto c: cluster){
    for(int i: c.index){
      notselectedindex.remove(i);
    }
  }
}

void ReadData::calDgCentrality(int dgthr){
  filterindex.clear();
  filteredge.clear();
  int vc[num];
  for(int i = 0;i < num;i++){
    vc[i] = 0;
  }
  for(pair<int,int> e :edge){
    int i = e.first;
    int j = e.second;
    vc[i]++;
    vc[j]++; 
  }
  bool filter[num];
  for(int i = 0;i< num;i++ ){
    if(vc[i] >= dgthr){
      filterindex.push_back(i);
      filter[i] = true;
    }
    else filter[i] = false;
  }
  //filterindexに入っていないものは描かないようにする。辺をどう減らすか?
  for(pair<int,int>e: edge){
    int i = e.first;
    int j = e.second;
    if(filter[i]&& filter[j])
      filteredge.push_back(e);
  }
  repairNIndex();
}
//複数の選択グループを全部解除  場合によっては既存の関数と統合
void ReadData::resetselected(){
  cluster.clear();
  notselectedindex.clear();
  filterindex.clear();
  filteredge.clear();
  for(int i = 0; i < num;i++){
    notselectedindex.push_back(i);
    filterindex.push_back(i);
  }
  for(pair<int,int>p:edge){
    filteredge.push_back(p);
  }
}

void ReadData::setCoodSelected(){
  isCoord = !isCoord;
}
void ReadData::setCood(int l,bool u){
  //どのように情報をもつのがよいのか? 
  //属性の番号をもつよりは何番目かを覚えたほうがよさそう
  if(u)
    selectedorder = l;
  else 
    selectedorder = l+1;
}
bool ReadData::containSelectedCood(int a){
  for(int i = max(selectedorder-1,0); i< min(selectedorder+2,atr);i++){
    if(order[i] == a)
      return true;
  }
  return false;
}



RGB ReadData::setColor(){
  //とりあえずこれで49色
  int size = cluster.size();
  float hue = (size % 10 )* 0.1;
  
  HSV hsv =  HSV(hue,0.7,0.7) ;
  return  HSVtoRGB(hsv);
} 

RGB ReadData::HSVtoRGB(HSV& hsv ){
    const float h = hsv.h;
    const float s = hsv.s;
    const float v = hsv.v;
    const float hueF = h * 6.0f;
    const int hueI = static_cast<int>(hueF);
    const float fr = hueF - hueI;
    const float m = v * (1.0f-s);
    const float n = v * (1.0f-s*fr);
    const float p = v * (1.0f-s*(1.0f-fr));

    RGB rgb;

  
    switch(hueI)
    {
      case 0: rgb.r = v; rgb.g = p; rgb.b = m; break;
      case 1: rgb.r = n; rgb.g = v; rgb.b = m; break;
      case 2: rgb.r = m; rgb.g = v; rgb.b = p; break;
      case 3: rgb.r = m; rgb.g = n; rgb.b = v; break;
      case 4: rgb.r = p; rgb.g = m; rgb.b = v; break;
      default: rgb.r = v; rgb.g = m; rgb.b = n; break;
    }
 
    return rgb;
  }


  


