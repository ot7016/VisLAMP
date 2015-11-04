#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <tuple>
#include <list>
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
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	void turnLenVar();
	std::vector<std::pair<int,int> > getEdge();
	std::vector<std::list<int> > getCluster();
	std::vector<int> getFIndex();
	std::list<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, std::vector<double> v);
	void setSIndex(std::list<int> v);
	void setCoodSelected();
	void setCood(int l,bool u);
	bool containSelectedCood(int a);
	void resetselected();
	double* A;
	double *evalue;
	double* evector;
	bool isTSP, isPCA,isLenVar, isCoord;
	int num, dim, atr, aginum;
	double** D;
	double* Dmax;
	double* Dmin;
	std::vector<std::string> name;
	std::vector<std::string>  atrname;	
	int* order;
	int selectedorder;
	//色をどのように決めるかが最大の問題　色はクラスターごとに一意であるべき
	std::vector<std::list<int> > cluster;

	private:
		double thr ;
		std::string dir;
		std::vector<int> filterindex;
		std::vector<std::pair<int,int> > edge;
		std::vector<int> selectedindex;
		std::list<int> notselectedindex;
};
#endif
