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
  delete[] name;
  delete[] order;
  delete[] atrname;
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
  atrname = new string[atr];
  string str;
  ifstream ifs1 (dir+ "-atrname.csv");
  if (ifs1.fail()) {
        cerr << "File Does not exist" << endl;
        exit(1);
    }
    int k = 0;
    getline(ifs1,str);
  while(getline(ifs1,str)){
    atrname[k] = str;
    k++;
}
    ifstream ifs2 (dir+ "-name.csv");
  if (ifs2.fail()) {
        cerr << "File Does not exist" << endl;
        exit(1);
    }
    getline(ifs2,str);
   name = new string[num];
   k = 0;
   while(getline(ifs2,str)){
    name[k] = str;
    k++; 
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
}
void ReadData::setOrder(int* o){
  order = new int[atr];
  for(int i = 0;i<atr;i++){
    order[i] = o[i];
  }
}
int ReadData::getOrder(int i){
  return order[i];
}

vector<std::pair<int, int> > ReadData::getEdge(){
  return edge;
}

void ReadData::turnLenVar(){
  isLenVar= !isLenVar;
}

vector<int>ReadData::getFIndex(){
  return filterindex;
}
vector<int> ReadData::getSIndex(){
  return selectedindex;
}
vector<int> ReadData::getNSIndex(){
  return notselectedindex;
}
//今は前の選択を保持しない形とする
void ReadData::setSIndex(int i){
  clearSIndex();
  selectedindex.push_back(i);
  for(int j = 0;j< i;j++){
    notselectedindex.push_back(j);
  }
  for(int j = i+1;j<num;j++){
    notselectedindex.push_back(j);
  }
}

void ReadData::setSIndex(int j, vector<double> v){
  clearSIndex();
  sort(v.begin(),v.end());
  for(int i = 0;i < num;i++){
    if( D[i][j] > v.at(0) && D[i][j] < v.at(1) )
      selectedindex.push_back(i);
  }
  for(int i = 0;i< num;i++){
        auto result = find(begin(selectedindex),end(selectedindex),i);
        if(result == end(selectedindex))
            notselectedindex.push_back(i);
        }
}
void ReadData::setSIndex(vector<int> v){
  clearSIndex();
  selectedindex = v;
  for(int i: v){
     auto result = find(begin(selectedindex),end(selectedindex),i);
        if(result == end(selectedindex))
            notselectedindex.push_back(i);
  }
}
void ReadData::clearSIndex(){
  selectedindex.clear();
  notselectedindex.clear();
  for(int i = 0; i<num;i++){
    notselectedindex.push_back(i);
  }
}
void ReadData::setCoodSelected(){
  isCood = !isCood;
}