#include <iostream>
#include "str.h"
#include "algo.h"
using namespace std;

void algo1(char *text,char *pattern)
{
	int n;
	startTimeCount();
	n=countPatterns(text,pattern);
	stopTimeCount();
	cout << n << endl;
}

void algo2(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);
	startTimeCount();

	/// omp parallel for
	for(int i=0;i<size;i++)
	{
		if(checkPattern(&text[i],pattern))
			{n++;}
	}

	stopTimeCount();
	cout << n << endl;
}

void algo3(char *text,char *pattern)
{
	int n=0;
	int size=strSize(text);
	startTimeCount();

	/// omp parallel for
	for(int i=0;i<4;i++)
	{
		int m=i*size/4;
		for(int j=0;j<size/4;j++)
		{
			if(checkPattern(&text[m+j],pattern))
				{n++;}
		}
		
	}

	stopTimeCount();
	cout << n << endl;
}

void algo4(char *text,char *pattern)
{
	int n[4];
	char **tab=transferSplit(text,4,4);
	startTimeCount();

	/// omp parallel for
	for(int i=0;i<4;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	stopTimeCount();
	int sum=0;
	for(int i=0;i<4;i++)
	{
		sum+=n[i];
	}
	cout << sum << endl;
}


void algo5(char *text,char *pattern)
{
	int n[16];
	char **tab=transferSplit(text,16,4);
	startTimeCount();

	/// omp parallel for limit 4
	for(int i=0;i<16;i++)
	{
		n[i]=countPatterns(tab[i],pattern);		
	}

	stopTimeCount();
	int sum=0;
	for(int i=0;i<16;i++)
	{
		sum+=n[i];
	}
	cout << sum << endl;
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

	algo5(text,pattern);

	system("pause");
	return 0;
}