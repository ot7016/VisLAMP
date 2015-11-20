#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <tuple>
#include <list>
#include <sys/types.h>
#include <dirent.h>
#include "TSP.hpp"
//#include "Color.hpp"

#ifndef rdata 
#define rdata 
using namespace std;

#define STATUS_OK 0
#define STATUS_ERROR 1
#define STATUS_NO_DISK_SPACE 2

struct RGB{
    RGB(){}
    RGB( float r_, float g_, float b_ ) : r(r_),g(g_),b(b_){}
    float r,g,b;  // [0.0f, 1.0f]
  };

  struct HSV
  {
    HSV(){}
    HSV( float h_, float s_, float v_ ) : h(h_),s(s_),v(v_){}
    float h;  // ... 0°==0.0f, 360°==1.0f ...
    float s,v;  // [0.0f, 1.0f]
  };
  struct S_Cluster{
	S_Cluster(){}
	S_Cluster(list<int> i,RGB r):rgb(r),index(i){}
	RGB rgb;
	list<int> index;

};



class ReadData{
	public:
		ReadData();
		~ReadData();
	void read(int id);
	void readfname();
	void readsetting(string dir);
	void readcood(string dir);
	void readevalue(string dir);
	void readoriginal(string dir);
	void readDist(string dir);
	void calEdge();
	void recalEdge(double t);
	double getevector(int i, int j);
	double getA(int i, int j);
	void setOrder(TSPsolver* ts);
	void setOrder(int* o);
	void turnLenVar();
	vector<pair<int,int> > getEdge();
	vector<S_Cluster > getCluster();
	list<int> getFIndex();
	list<int> getNSIndex();
	void setSIndex(int i);
	void setSIndex(int j, vector<double> v, int clickid);
	void setSIndex(list<int> v,int clickid);
	void repairNIndex();
	void setCoodSelected();
	void setCood(int l,bool u);
	bool containSelectedCood(int a);
	void resetselected();
	RGB HSVtoRGB(HSV& hsv );
	RGB setColor();
    void calDgCentrality(int dgthr);
    void clearall();
	double* A;
	double* alldist;
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
	double thr ,pthr;
	//色をどのように決めるかが最大の問題　色はクラスターごとに一意であるべき
	vector<S_Cluster > cluster;
	list<int> filterindex;
	vector<pair<int,int> > edge;
	vector<pair<int,int> > filteredge;
	//std::vector<list<int> > cluster;
	vector<string> dataname;
	private:
		const int DIST_SIZE = 2000;
		
		

		list<int> notselectedindex;
		int lastclickid;

};
#endif