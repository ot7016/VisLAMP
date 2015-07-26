#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <math.h>
#include <sstream>

#ifndef tsp 
#define tsp
//typedef struct {
//std::vector<int> order;
//float** dis;
//float length;
/*
  item(float** d, int a){
  	for(int i = 0;i < a; i++){
  		dis[i][0] = d[i][0];
  		dis[i][1] = d[i][1];
  		order[i] = i;
  	}
  }
  */
//}forindex;


class TSPsolver {
public:
	TSPsolver(float** v, int a);
	~TSPsolver();
	void solve();
	std::vector<int> solveTSP1(int index);
	bool iscontain(std::vector<int> v, int n);
private: 
	float** dist;  
	int atr;
	float* length;
	


};
#endif