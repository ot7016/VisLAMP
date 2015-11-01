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
	void calEdge();
	double getevector(int i, int j);
	double getA(int i, int j);
	int getOrder(int i);
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	void turnLenVar();
	std::vector<std::pair<int,int> > getEdge();
	std::vector<int> getFIndex();
	std::vector<int> getSIndex();
	std::vector<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, std::vector<double> v);
	void setSIndex(std::vector<int> v);
	void clearSIndex();
	void setCoodSelected();
	double* A;
	double *evalue;
	double* evector;
	bool isTSP, isPCA,isLenVar, isCood;
	int num, dim, atr, aginum;
	double** D;
	double* Dmax;
	double* Dmin;
	std::string* name;
	std::string* atrname;	
	private:
		double thr ;
		std::string dir;
		int* order;
		std::vector<std::pair<int,int> > edge;
		std::vector<int> filterindex;
		std::vector<int> selectedindex;
		std::vector<int> notselectedindex;
};
#endif
