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
using namespace std;

#define STATUS_OK 0
#define STATUS_ERROR 1
#define STATUS_NO_DISK_SPACE 2

class ReadData{
	public:
		ReadData();
		~ReadData();
	void read(string dir);
	void readevalue(string dir);
	void readoriginal(string dir);
	void readDist(string dir);
	void calEdge();
	double getevector(int i, int j);
	double getA(int i, int j);
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	void turnLenVar();
	vector<pair<int,int> > getEdge();
	vector<list<int> > getCluster();
	vector<int> getFIndex();
	list<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, vector<double> v);
	void setSIndex(list<int> v);
	void setCoodSelected();
	void setCood(int l,bool u);
	bool containSelectedCood(int a);
	void resetselected();
	double* A;
	//double* alldist;
	double *evalue;
	double* evector;
	bool isTSP, isPCA,isLenVar, isCoord;
	int num, dim, atr, aginum;
	double** D;
	double* Dmax;
	double* Dmin;
	vector<string> name;
	vector<string>  atrname;	
	int* order;
	int selectedorder;
	//色をどのように決めるかが最大の問題　色はクラスターごとに一意であるべき
	vector<list<int> > cluster;

	private:
		const int DIST_SIZE = 2000;
		double thr ;
		//std::string dir;
		vector<int> filterindex;
		vector<pair<int,int> > edge;
		vector<int> selectedindex;
		list<int> notselectedindex;
};
#endif
