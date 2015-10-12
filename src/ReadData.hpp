#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <tuple>
#include "TSP.hpp"

#ifndef rdata 
#define rdata 

class ReadData{
	public:
		ReadData();
		~ReadData();
		//typedef std::pair<int,int> npair;

	std::vector<std::string> split(const std::string &str,char delim);
	void read();
	void readevalue();
	void readoriginal();
	void readadjency();
	float getA();
	int getnum();
	int getdim();
	int getatr();
	int getnumatr();
	float getevalue(int i);
	float getevector(int i,int j);
	float getA(int i, int j);
	float getD(int i,int j);
	std::string getName(int i);
	std::string getAtrName(int i);
	float getAmin(int i);
	float getAmax(int i);
	float getDmax(int i);
	float getDmin(int i);
	int getOrder(int i);
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	bool isTSP();
	void setTSP(bool b);
	bool isLenVar();
	void turnLenVar();
	std::vector<std::pair<int,int> > getEdge();
	std::vector<int> getFIndex();
	std::vector<int> getSIndex();
	std::vector<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, std::vector<float> v);
	void clearSIndex();

	private:
		float thr ;
		std::string dir;
		float *evalue;
		float** evector;
		float** A;
		float** D;
		std::string* name;
		std::string* atrname;
		float* Amin;
		float* Amax;
		float* Dmax;
		float* Dmin;
		int* order;
		int num,dim,atr;
		bool ists ;
		bool lengthvariable;
		std::vector<std::pair<int,int> > edge;
		std::vector<int> filterindex;
		std::vector<int> selectedindex;
		std::vector<int> notselectedindex;
};
#endif
