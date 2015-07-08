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

}
ReadData::~ReadData(){
  delete[] A;
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
	ifstream ifs(dir+"-scl.csv");
	string str;
	if (ifs.fail()){
        cerr << "失敗" << endl;
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
    ifstream ifs2(dir+"-eigen.csv");
	if (ifs2.fail())
    {
        cerr << "失敗" << endl;
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

int ReadData::getnum(){
	return num;
}
int ReadData::getdim(){
	return dim;
}
float ReadData::getevalue(int i){
	return evalue[i];
}
float ReadData::getA(int i, int j){
	return A[i][j];
}
float ReadData::getAmin(int i){
  return Amin[i];
}
float ReadData::getAmax(int i){
  return Amax[i];
}

