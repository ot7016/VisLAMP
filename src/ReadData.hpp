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
	float getA();
	int getnum();
	int getdim();
	float getevalue(int i);
	float getA(int i, int j);
	float getAmin(int i);
	float getAmax(int i);

	private:
		std::string dir;
		float *evalue;
		float** A;
		float* Amin;
		float* Amax;
	//	float** e;
	//	float** B; 
		int num,dim;




};
#endif
