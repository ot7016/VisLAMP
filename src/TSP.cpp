#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "TSP.hpp"

using namespace std;


TSPsolver::TSPsolver(float** v,int a){
	atr = a;
	//距離行列をつくる
	dist = new float*[atr];
	for(int i = 0; i< atr; i++){
		dist[i] = new float[atr];
		for(int j = 0; j < atr; j++){ 
			dist[i][j] = sqrt(pow(v[i][0] - v[j][0],2) + pow(v[i][1] - v[j][1],2));
		}
	}
}
void TSPsolver::solve(){
	
	length = 0;
	solveTSP1(0);
	
	
	//copy(ans.at(minindex).begin(),ans.at(minindex).end(),back_inserter(noworder));
	return ;

}
// 始点は決まっている
void TSPsolver::solveTSP1(int index){
 	order = vector <int>();
	order.push_back(index);
	    //data =  minplus(data,dist[index]); //ここで最初のをきめる
	while(order.size() < atr ){
		int last = order.at(order.size()-1);
		int num = atr -order.size();
		int notcontain[num];
		int j = 0;
		for(int i = 0; i< atr ; i++){
			if(!iscontain(order,i)){
				notcontain[j] = i; 
				j++;
			}
		}
		float min = dist[last][notcontain[0]];
		int minindex = notcontain[0];
	    for(int i = 0; i< num; i++){
	    	int nc = notcontain[i];
	    	float d = dist[last][nc];
	    	if(min >= d ){
	    		min = d;
				minindex = nc;
			}
		}
		length += min;
		order.push_back(minindex);
		//cerr << minindex << endl;
	}
	
	return ;
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
//一つ前との距離を返す
float TSPsolver::getlength(int index){
	if(index >0){
		int pre = 0;
		for(int i = 0; i< atr;i++){
			if(index == order.at(i))
				pre = i;
		}
		return dist[order.at(pre)][order.at(pre-1)];
	}
	else return -1;
}
float TSPsolver::getsumlength(){
	return mints ;	
}
int TSPsolver::getorder(int i){
	return order.at(i);
}


