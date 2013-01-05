#include <iostream>
#include <cstdio>
#include <conio.h>
#include <omp.h>
#include "str.h"
#include "algo.h"
using namespace std;

int algo1(char *text,char *pattern)
{
	int n;
	n=countPatterns(text,pattern);
	return n;
}

int algo2(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);

#pragma omp parallel for
	for(int i=0;i<size;i++)
	{
		if(checkPattern(&text[i],pattern))
			{n++;}
	}
	
	return n;
}

int algo3(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);

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

	return n;
}

int algo4(char *text,char *pattern)
{
	int tsize=strSize(text);
	int psize=strSize(pattern);
	int s=4;
	int *n=new int[s];
	char **tab=transferSplit(text,s,psize-1);

#pragma omp parallel for
	for(int i=0;i<s;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	int sum=0;
	for(int i=0;i<s;i++)
	{
		sum+=n[i];
	}
	return sum;
}


int algo5(char *text,char *pattern)
{
	int tsize=strSize(text);
	int psize=strSize(pattern);
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	int *n=new int[s];
	char **tab=transferSplit(text,s,psize-1);

#pragma omp parallel for
	for(int i=0;i<s;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	int sum=0;
	for(int i=0;i<s;i++)
	{
		sum+=n[i];
	}
	return sum;
}

int algo6(char *text,char *pattern)
{
	int tsize=strSize(text);
	int psize=strSize(pattern);
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	int *n=new int[s];
	char **tab=transferSplit(text,s,psize-1);

#pragma omp parallel for num_threads(4)
	for(int i=0;i<s;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}


	int sum=0;
	for(int i=0;i<s;i++)
	{
		sum+=n[i];
	}
	return sum;
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
	char *text,*pattern;
	loadInstance("AC-382-AAAAAAAAAAAABAAAAA",&text,&pattern);
	//loadInstance("AM-9-EAAAA",&text,&pattern);
	int rep;
	/*
	rep=algo1(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}*/
	
	/*
	rep=algo2(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}
	
	rep=algo3(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}
	
	
	rep=algo4(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}
	
	
	rep=algo5(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}
	*/
	
	rep=algo6(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		getchar();
	}
	
	return 0;
}