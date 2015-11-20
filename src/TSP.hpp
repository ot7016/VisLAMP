#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <iterator>

#ifndef tsp 
#define tsp
//typedef struct {
//std::vector<int> order;
//double** dis;
//double length;
/*
  item(double** d, int a){
  	for(int i = 0;i < a; i++){
  		dis[i][0] = d[i][0];
  		dis[i][1] = d[i][1];
  		order[i] = i;
  	}
  }
  */
//}forindex;


class TSPsolver{
public:
	TSPsolver(double** v, int a);
	~TSPsolver();
	void solve();
	void solveTSP1(int index);
	bool iscontain(std::vector<int> v, int n);
	double getlength(int i);
	double getsumlength();
	int getorder(int i);
private: 
	double** dist;  
	int atr;
	double length;
	double mints;
	std::vector<int> order;
};
#endif