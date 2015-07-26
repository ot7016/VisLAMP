#include <stdio.h>
#include <math.h>
#include "TSP.hpp"

using namespace std;


TSPsolver::TSPsolver(float** v,int a){
	atr = a;
	//距離行列をつくる
	dist = new float*[atr];
	for(int i = 0; i< atr; i++){
		dist[i] = new float[atr];
		for(int j = 0; j < atr; j++){ 
			dist[i][j] = pow(v[i][0] - v[j][0],2) + pow(v[i][1] - v[j][1],2);
		}
	}
}
void TSPsolver::solve(){
	vector< vector<int> > ans ;
	length = new float[atr];

	for(int i = 0; i< atr; i++){
		ans.push_back(solveTSP1(i));
	}

}
// 始点は決まっている
 vector<int> TSPsolver::solveTSP1(int index){
	vector<int> order = vector<int>();
	order.push_back(index);
	    //data =  minplus(data,dist[index]); //ここで最初のをきめる
	while(order.size() < atr ){
		int last = order.at(order.size()-1);
		int num = atr -order.size();
		int notcontain[num];
		int j = 0;
		for(int i = 0; i< atr ; i++){
			if(!iscontain(order,i))
				notcontain[j] = i; 
		}
		float min = dist[last][notcontain[0]];
		int minindex = notcontain[0];
	    for(int i = 0; i< num; i++){
	    	int d = dist[last][i];
	    	if(min >= d ){
	    		min = d;
				minindex = i;
			}
		}
		length[index] += min;
		order.push_back(minindex);
	}
	return order;
}
bool TSPsolver::iscontain(vector<int> v,int n){
	for(int i = 0; i < v.size(); i++){
		if(v.at(i) == n)
			return true;
	}
	return false; 
}

TSPsolver::~TSPsolver(){
	delete[] dist;
}


