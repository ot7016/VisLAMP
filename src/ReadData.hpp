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
	void read();
	void readevalue();
	void readoriginal();
	void readadjency();
	double getA();
	int getnum();
	int getdim();
	int getatr();
	int getnumatr();
	double getevalue(int i);
	double getevector(int i, int j);
	double getA(int i, int j);
	double getD(int i,int j);
	std::string getName(int i);
	std::string getAtrName(int i);
	double getAmin(int i);
	double getAmax(int i);
	double getDmax(int i);
	double getDmin(int i);
	int getOrder(int i);
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	bool isTSP();
	void setTSP(bool b);
	bool isPCA();
	bool isLenVar();
	void turnLenVar();
	std::vector<std::pair<int,int> > getEdge();
	std::vector<int> getFIndex();
	std::vector<int> getSIndex();
	std::vector<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, std::vector<double> v);
	void setSIndex(std::vector<int> v);
	void clearSIndex();
	double* A;
	double* evector;
	private:
		double thr ;
		std::string dir;
		double *evalue;
		double** D;
		std::string* name;
		std::string* atrname;
		double* Amin;
		double* Amax;
		double* Dmax;
		double* Dmin;
		int* order;
		int num,dim,atr;
		bool ists ;
		bool lengthvariable;
		bool iPCA;
		std::vector<std::pair<int,int> > edge;
		std::vector<int> filterindex;
		std::vector<int> selectedindex;
		std::vector<int> notselectedindex;
};
#endif
