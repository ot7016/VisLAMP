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
	read();	
  readevalue();
  readoriginal();

}
ReadData::~ReadData(){
  delete[] A;
  delete[] D;
  delete[] name;
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
        dim = v.size(); //一度のみ
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
      
      
/*
      evalue = new float[dim];
       cerr << "evalue" << endl;
  for(int i = 0; i <dim; i++){
    double sum = 0;
    for (int j = 0; j < num; j++){
      sum = sum + pow(A[j][i],2); 
    }
    evalue[i] = sqrt(sum);
     cerr << evalue[i] << endl;
  }    
   cerr << "evalue end" << endl;
   */
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
      //int index1 = i;
      //int index2 = j;
      float d = D[i][j];
      if(Dmin[j] >d)
        Dmin[j] = d;
      if(Dmax[j] < d)
        Dmax[j] = d; 
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
float ReadData::getevalue(int i){
	return evalue[i];
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