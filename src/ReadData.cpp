#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "ReadData.hpp"

using namespace std;

ReadData::ReadData(){
	ifstream ifs("../data/input.txt");
	if (ifs.fail()){
        cerr << "失敗" << endl;
        exit(1);
    }
	getline(ifs,dir);
    cerr << dir << endl;
    string ths;
  getline(ifs,ths);
  thr = stof(ths);
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
	//仮に入れて置く場所
	float** T;
	T = new float*[10000]; 
	//ファイルを読み込みAとevalueを作成
	//ここでAとevalueを動的確保
	//汚いのであとで整理
	int n2 = 0;
	//Aの読み取り
	ifstream ifs(dir+"-cood.csv");
	string str;
	if (ifs.fail()){
        cerr << "cood失敗" << endl;
        exit(1);
    }
    int i = 0;
    getline(ifs,str);
    bool notread = true;
    while (getline(ifs, str)){
      vector<string> v = split(str,',');
      T[i] = new float[v.size()];
      int j =0;  
      for(string s : v){
      	//分割結果を一旦格納
      	T[i][j] = stod(s);
      	j++;
      }
      if(notread){
        dim = v.size(); //一度のみ   この時点で高次元行列を作っているので dim >0
        notread = false;
    }
      i++; 
    }
    num = i;
   
   Amax = new float[dim];
   Amin = new float[dim];
   for(i = 0;i< dim;i++){
    Amax[i] = 0;
    Amin[i] = 0;
   }

    //Aに移す
    A = new float*[num];
    for(i = 0;i<num;i++){
    	A[i] = new float[dim];
    	for(int k = 0;k< dim;k++){
    		A[i][k] = T[i][k];
        if(A[i][k] > Amax[k])
          Amax[k] = A[i][k];
        if(A[i][k] < Amin[k])
          Amin[k] = A[i][k];
      }
    }
    delete[] T;

}
void ReadData::readevalue(){

  
	string str;
    ifstream ifs2(dir+"-evalue.csv");
	if (ifs2.fail())
    {
        cerr << "evalue失敗" << endl;
        exit(1);
    }
    getline(ifs2,str);
    getline(ifs2, str);
      vector<string> v2 = split(str,',');
       int i =0;  
     int m  = v2.size();
      evalue = new float[m];
      for(string s : v2){
      	//分割結果をeに格納
      	evalue[i] = stod(s);
      	i++;
      }
      ifstream ifs3(dir+"-evector.csv");
      evector = new float*[m];
      getline(ifs3,str);
      i = 0;
      while(getline(ifs3,str)){
        vector<string> v = split(str,',');
        evector[i] = new float[num];
        for(int j = 0;j < num;j++){
          evector[i][j] = stod(v.at(j));
        }
        i++;
      }
}

 vector<string> ReadData::split(const std::string &str,char delim){
    vector<string> v;
    stringstream ss(str);
    string buffer;
    while( getline(ss, buffer, delim) ){
        v.push_back(buffer);
    }
    return v;
}

void ReadData::readoriginal(){
  ifstream ifs(dir+"-original.csv");
  string str;
  if(ifs.fail()){
    cerr << "original失敗" << endl;
        exit(1);
  }
  //これ以前にAを読み込んでいるので numはもう決まっている がこれは 本来のnum + 属性分になる

  //最初に1行目(ラベル)を読み込んで　属性数を決定 
  getline(ifs,str);
  vector<string> v = split(str,',');
  atr = v.size()-1;
  atrname = new string[atr];
  for(int i = 0 ;i< atr; i++){
    atrname[i] =  v.at(i);
  }

  num = num -atr;
  D = new float*[num];
  name = new string[num];
  int k = 0;
  while(getline(ifs,str)){
    vector<string> v = split(str,',');
     D[k] = new float[atr];
    //carsが最後に名前がきているので暫定的にこうする
    for(int j = 0;j<atr;j++){
      D[k][j] = stod(v.at(j));
    }
    name[k] = v.at(atr);
    k++;
  }
  Dmin = new float[atr];
  Dmax = new float[atr];
  for(int i = 0; i< atr; i++){
    Dmin[i] = D[0][i];
    Dmax[i] = D[0][i];   
  }

  for(int i = 0; i< num; i++){
    for(int j = 0; j<atr; j++){
      float d = D[i][j];
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


  ifstream ifs(dir+"-adjency.csv");
  string str;
  if(ifs.fail()){
    cerr << "original失敗" << endl;
        exit(1);
  }
  getline(ifs,str);
  int k = 0;
  while(getline(ifs,str)){
    vector<string> v = split(str,',');
    for(int i = k; i< v.size();i++){
      double ad = stod(v.at(i));
      if(ad != 0 && ad < thr )
        edge.push_back(pair<int,int>(k,i)); 
    }
    k++;
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
float ReadData::getevalue(int i){
	return evalue[i];
}
float ReadData::getevector(int i, int j){
  return evector[i][j];
}
float ReadData::getA(int i, int j){
	return A[i][j];
}
float ReadData::getD(int i,int j){
  return D[i][j];
}
string ReadData::getName(int i){
  return name[i];
}
string ReadData::getAtrName(int i){
  return atrname[i];
}
float ReadData::getAmin(int i){
  return Amin[i];
}
float ReadData::getAmax(int i){
  return Amax[i];
}
float ReadData::getDmax(int i){
  return Dmax[i];
}
float ReadData::getDmin(int i){
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
//void ReadData::addFIndex(int i){
//  filterindex.push_back(i);
//}
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

void ReadData::setSIndex(int j, vector<float> v){
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
void ReadData::clearSIndex(){
  selectedindex.clear();
  notselectedindex.clear();
  for(int i = 0; i<num;i++){
    notselectedindex.push_back(i);
  }
}


