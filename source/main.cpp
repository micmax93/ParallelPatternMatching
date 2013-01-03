#include <iostream>
#include <cstdio>
#include <conio.h>
#include "str.h"
#include "algo.h"
using namespace std;

void algo1(char *text,char *pattern)
{
	int n;
	//startTimeCount();
	n=countPatterns(text,pattern);
	/*
	stopTimeCount();
	cout << n << "\t time: " << getTime() << endl;
	*/
}

void algo2(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);
	// startTimeCount();

#pragma omp parallel for
	for(int i=0;i<size;i++)
	{
		if(checkPattern(&text[i],pattern))
			{n++;}
	}

	/*
	stopTimeCount();
	cout << n << "\t time: " << getTime() << endl;
	*/
}

void algo3(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);
	//startTimeCount();

#pragma omp parallel for
	for(int i=0;i<4;i++)
	{
		int m=i*size/4;
		for(int j=0;j<size/4;j++)
		{
			if(checkPattern(&text[m+j],pattern))
				{n++;}
		}
		
	}

	/*
	stopTimeCount();
	cout << n  << "\t time: " << getTime() << endl;
	*/
}

void algo4(char *text,char *pattern)
{
	int n[4];
	char **tab=transferSplit(text,4,4);
	//startTimeCount();

#pragma omp parallel for
	for(int i=0;i<4;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	/*
	stopTimeCount();
	int sum=0;
	for(int i=0;i<4;i++)
	{
		sum+=n[i];
	}
	cout << sum << "\t time: " << getTime() << endl;
	*/
}


void algo5(char *text,char *pattern)
{
	int tsize=strSize(text);
	int psize=strSize(pattern);
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	int *n=new int[s];
	char **tab=transferSplit(text,s,psize-1);
	//startTimeCount();

#pragma omp parallel for
	for(int i=0;i<s;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	/*
	stopTimeCount();
	int sum=0;
	for(int i=0;i<s;i++)
	{
		sum+=n[i];
	}
	cout << sum << "\t time: " << getTime() << endl;
	*/
}

void algo6(char *text,char *pattern)
{
	int tsize=strSize(text);
	int psize=strSize(pattern);
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	int *n=new int[s];
	char **tab=transferSplit(text,s,psize-1);
	//startTimeCount();

#pragma omp parallel for num_threads(4)
	for(int i=0;i<s;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	/*
	stopTimeCount();
	int sum=0;
	for(int i=0;i<s;i++)
	{
		sum+=n[i];
	}
	cout << sum << "\t time: " << getTime() << endl;
	*/
}

int repeatAndApproximateTime(void (*algo) (char*, char*), char *text, char *pattern, int iter = 100)
{
	if (iter > 500 || iter < 1)
		iter = 100;
	double sum = 0;

	for(int i=0; i<iter; ++i)
	{
		startTimeCount();
		algo(text, pattern);
		stopTimeCount();
		if (i==0)
			sum = getTime();
		else
			sum = (sum + getTime())/2;
	}

	return sum;
}

int main(int argc,char **argv)
{
	if(argc<2)
	{
		cout << "Za ma³a iloœæ parametrów" << endl;
		return 0;
	}

	char *text,*pattern;
	loadInstance(argv[1],&text,&pattern);
	
	int iterNum = 500;
	if (argc == 3)
		iterNum = atoi(argv[2]);

	cout << "ALGO1: ";
	//algo1(text,pattern);
	cout << repeatAndApproximateTime(algo1, text, pattern, iterNum);

	cout << "\nALGO2: ";
	//algo2(text,pattern);
	cout << repeatAndApproximateTime(algo2, text, pattern, iterNum);

	cout << "\nALGO3: ";
	//algo3(text,pattern);
	cout << repeatAndApproximateTime(algo3, text, pattern, iterNum);

	cout << "\nALGO4: ";
	//algo4(text,pattern);
	cout << repeatAndApproximateTime(algo4, text, pattern, iterNum);

	cout << "\nALGO5: ";
	//algo5(text,pattern);
	cout << repeatAndApproximateTime(algo5, text, pattern, iterNum);

	cout << "\nALGO6: ";
	//algo6(text,pattern);
	cout << repeatAndApproximateTime(algo6, text, pattern, iterNum);

	//getch();

	return 0;
}