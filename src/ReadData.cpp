#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "ReadData.hpp"

using namespace std;

/****
最初のinputでnum dim atrの大きさを読み込んで決める
num 項目の数
atr 属性数
dim 高次元配置の次元数 

****/

ReadData::ReadData(){
	ifstream ifs("../data/input.txt");
	if (ifs.fail()){
        cerr << "失敗" << endl;
        exit(1);
    }
	getline(ifs,dir);
    cerr << dir << endl;
    string str;
  
  getline(ifs,str);
  thr = stod(str);
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
	read();	
  readevalue();
  calEdge();
  readoriginal();
  order = new int[atr];
  for(int i = 0;i<atr;i++){
    order[i] = i;
  }

}
ReadData::~ReadData(){
  delete[] A;
  delete[] D;
  delete[] evector;
  delete[] order;
  delete[] evalue;
  }

void ReadData::read(){
	
	
	//ここでAを動的確保
	//Aの読み取り
	fstream fs(dir+"-cood.dat",ios::in | ios::binary);
	string str;
	if (!fs){
        cerr << "cood失敗" << endl;
        exit(1);
    }
    // A は (num+atr) * dimの行列 
    A = new double[aginum * dim]; 
    fs.seekg(0);
    fs.read((char*) A, sizeof (double) * aginum  * dim);

}
void ReadData::readevalue(){
  fstream fs2(dir+"-evalue.dat",ios::in | ios::binary);
	if (!fs2){
    cerr << "evalue失敗" << endl;
     exit(1);
  }  
  double nums1[dim];

  evalue = new double[dim];
  fs2.seekg(0);
  fs2.read((char*) nums1, sizeof (double)* dim);
  for(int i = 0; i< dim ;i++){
  	evalue[i] = nums1[i];
  }
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


void ReadData::readoriginal(){

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
      notselectedindex.push_back(i);
  }
}

void ReadData::calEdge(){
  for(int i = 0; i < aginum;i++){
    for(int j = i+1; j < aginum; j++){
      double s = 0;
      for(int k = 0; k < dim; k++ ){
        s = s + pow(getA(i,k) - getA(j,k) ,2);
      }
      s = sqrt(s);
      if(s!= 0 && s < thr)
      edge.push_back(pair<int,int>(i,j));
    }
    
  }
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
vector<list<int> > ReadData::getCluster(){
  return cluster;
}

vector<int>ReadData::getFIndex(){
  return filterindex;
}

list<int> ReadData::getNSIndex(){
  return notselectedindex;
}
//今は前の選択を保持しない形とする
void ReadData::setSIndex(int i){
  list<int> selected;
  selected.push_back(i);
  cluster.push_back(selected);
  notselectedindex.remove(i);
}

void ReadData::setSIndex(int j, vector<double> v){
  list<int> selected;
  sort(v.begin(),v.end());
  for(int i = 0;i < num;i++){
    if( D[i][j] > v.at(0) && D[i][j] < v.at(1) )
      selected.push_back(i);
  }
  cluster.push_back(selected);
  for(int i : selected){
    notselectedindex.remove(i);     
  }
}
//リストで実装し直すか?
void ReadData::setSIndex(list<int> v){
  cluster.push_back(v);
 // auto ite ;
    for(int i:v){
      notselectedindex.remove(i);     
    }
  
}

//複数の選択グループを全部解除  場合によっては既存の関数と統合
void ReadData::resetselected(){
  cluster.clear();
  notselectedindex.clear();
  for(int i = 0; i < num;i++){
    notselectedindex.push_back(i);
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

