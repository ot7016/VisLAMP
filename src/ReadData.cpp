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
  ists = false;
  lengthvariable = true;
	read();	
  readevalue();
  readadjency();
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
  delete[] Amin;
  delete[] Amax;
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
    A = new double*[num+atr];
    double nums[num+atr][dim]; 
    fs.seekg(0);
    fs.read((char*) nums, sizeof (double) * (num + atr) * dim);
   
   Amax = new double[dim];
   Amin = new double[dim];
   for(int i = 0;i< dim;i++){
    Amax[i] = 0;
    Amin[i] = 0;
   }

    //Aに移す
    for(int i = 0; i<num+atr; i++){
    	A[i] = new double[dim];
    	for(int j = 0; j< dim; j++){
    		A[i][j] = nums[i][j];
        if(A[i][j] > Amax[j])
          Amax[j] = A[i][j];
        if(A[i][j] < Amin[j])
          Amin[j] = A[i][j];
      }
    }

}
void ReadData::readevalue(){

    fstream fs2(dir+"-evalue.dat",ios::in | ios::binary);
	if (!fs2)
    {
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
     /*
      fstream fs3(dir+"-evector.dat",ios::in | ios::binary);
      if (!fs3) {
        cerr << "evector file Does not exist" << endl;
      }
      evector = new double*[dim];
      double nums2[dim][num];
      fs3.seekg(0);
      fs3.read((char*) nums2, sizeof (double) * dim * num);
      for(int i = 0;i< dim;i++ ){
        evector[i] = new double[num];
        for(int j = 0;j < num;j++){
          evector[i][j] = nums2[i][j];
        }
      }
      */
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
void ReadData::readadjency(){


  fstream fs(dir+"-adjency.dat",ios::in | ios::binary);
  string str;
  if(!fs){
    cerr << "original失敗" << endl;
        exit(1);
  }
  double nums[num][num]; 
  fs.seekg(0);
    fs.read((char*) nums, sizeof (double) * num * num);
  for(int i = 0; i< num;i++){
    for(int j = i+1; j< num;j++){
      double ad = nums[i][j];
      if(ad != 0 && ad < thr )
        edge.push_back(pair<int,int>(j,i)); 
    }
  }

}

int ReadData::getnum(){
	return num;
}
int ReadData::getdim(){
	return dim;
}
int ReadData::getatr(){
  return atr;
}
int ReadData::getnumatr(){
  return num+atr;
}
double ReadData::getevalue(int i){
	return evalue[i];
}
/*
double ReadData::getevector(int i, int j){
  return evector[i][j];
}
*/
double ReadData::getA(int i, int j){
	return A[i][j];
}
double ReadData::getD(int i,int j){
  return D[i][j];
}
string ReadData::getName(int i){
  return name[i];
}
string ReadData::getAtrName(int i){
  return atrname[i];
}
double ReadData::getAmin(int i){
  return Amin[i];
}
double ReadData::getAmax(int i){
  return Amax[i];
}
double ReadData::getDmax(int i){
  return Dmax[i];
}
double ReadData::getDmin(int i){
  return Dmin[i];
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
bool ReadData::isTSP(){
  return ists;
}
vector<std::pair<int, int> > ReadData::getEdge(){
  return edge;
}
void ReadData::setTSP(bool b){
  ists = b;
}
bool ReadData::isLenVar(){
  return lengthvariable;
}
void ReadData::turnLenVar(){
  lengthvariable = !lengthvariable;
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


