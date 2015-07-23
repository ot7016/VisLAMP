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

#ifndef rdata 
#define rdata 

class ReadData{
	public:
		ReadData();
		~ReadData();

	std::vector<std::string> split(const std::string &str,char delim);
	void read();
	void readevalue();
	void readoriginal();
	float getA();
	int getnum();
	int getdim();
	int getatr();
	int getnumatr();
	float getevalue(int i);
	float getA(int i, int j);
	float getD(int i,int j);
	std::string getName(int i);
	float getAmin(int i);
	float getAmax(int i);
	float getDmax(int i);
	float getDmin(int i);

	private:
		std::string dir;
		float *evalue;
		float** A;
		float** D;
		std::string* name;
		float* Amin;
		float* Amax;
		float* Dmax;
		float* Dmin;
	//	float** e;
	//	float** B; 
		int num,dim,atr;




};
#endif
