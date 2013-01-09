#include <iostream>
#include <cstdio>
#include <conio.h>
#include <omp.h>
#include "str.h"
using namespace std;

int algo1(char *text,char *pattern)
{
	int n=0;
	char *tptr,*pptr;
	while(*text!=NULL)
	{
		if(*text==*pattern)
		{
			tptr=text+1;
			pptr=pattern+1;
			while(true)
			{
				if(*pptr==NULL)
				{
					n++;
					break;
				}
				if(*tptr!=*pptr)
				{
					break;
				}
				tptr++;
				pptr++;
			}
		}
		text++;
	}
	return n;
}

int algo2(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for
	for(int i=0;i<tsize;i++)
	{
		char *tptr,*pptr;
		if(text[i]==*pattern)
		{
			tptr=&text[i+1];
			pptr=pattern+1;
			while(true)
			{
				if(*pptr==NULL)
				{
					++n;
					break;
				}
				if(*tptr!=*pptr)
				{
					break;
				}
				tptr++;
				pptr++;
			}
		}
	}
	
	return n;
}

int algo3(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for
	for(int i=0;i<4;i++)
	{
		char *tptr,*pptr;
		int m=i*tsize/4;
		for(int j=0;j<tsize/4;j++)
		{
			if(text[m+j]==*pattern)
			{
				tptr=&text[m+j+1];
				pptr=pattern+1;
				while(true)
				{
					if(*pptr==NULL)
					{
						++n;
						break;
					}
					if(*tptr!=*pptr)
					{
						break;
					}
					tptr++;
					pptr++;
				}
			}
		}
	}

	return n;
}

int algo4(char **ttab,char *pattern)
{
	int s=4;
	int n=0;

	#pragma omp parallel for shared(n)
	for(int i=0;i<s;i++)
	{
		int subsum=0;
		char *tptr,*pptr;
		char *text=ttab[i];

		while(*text!=NULL)
		{
			if(*text==*pattern)
			{
				tptr=text+1;
				pptr=pattern+1;
				while(true)
				{
					if(*pptr==NULL)
					{
						subsum++;
						break;
					}
					if(*tptr!=*pptr)
					{
						break;
					}
					tptr++;
					pptr++;
				}
			}
			text++;
		}
		
		#pragma omp atomic
		n+=subsum;
	}

	return n;
}

int algo5(char **ttab,char *pattern,int s)
{
	int n=0;
	#pragma omp parallel for
	for(int i=0;i<s;i++)
	{
		int subsum=0;
		char *tptr,*pptr;
		char *text=ttab[i];

		while(*text!=NULL)
		{
			if(*text==*pattern)
			{
				tptr=text+1;
				pptr=pattern+1;
				while(true)
				{
					if(*pptr==NULL)
					{
						subsum++;
						break;
					}
					if(*tptr!=*pptr)
					{
						break;
					}
					tptr++;
					pptr++;
				}
			}
			text++;
		}	
		
		#pragma omp atomic
		n+=subsum;
	}

	return n;
}

int algo6(char **ttab,char *pattern,int s)
{
	int n=0;

	#pragma omp parallel for num_threads(4)
	for(int i=0;i<s;i++)
	{
		int subsum=0;
		char *tptr,*pptr;
		char *text=ttab[i];

		while(*text!=NULL)
		{
			if(*text==*pattern)
			{
				tptr=text+1;
				pptr=pattern+1;
				while(true)
				{
					if(*pptr==NULL)
					{
						subsum++;
						break;
					}
					if(*tptr!=*pptr)
					{
						break;
					}
					tptr++;
					pptr++;
				}
			}
			text++;
		}	
		
		#pragma omp atomic
		n+=subsum;
	}
	return n;
}


int main(int argc,char **argv)
{
	char *text,*pattern;
	//loadInstance("AC-382-AAAAAAAAAAAABAAAAA",&text,&pattern);
	loadInstance("AM-9-EAAAA",&text,&pattern);
	int rep;
	int tsize=strSize(text);
	int psize=strSize(pattern);
	//cout << "Expected " << expected << endl;

	rep=algo1(text,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo1=" << rep << endl;
		getchar();
	}
	
	rep=algo2(text,pattern,tsize);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo2=" << rep << endl;
		getchar();
	}
	
	rep=algo3(text,pattern,tsize);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo3=" << rep << endl;
		getchar();
	}
	
	
	char **tab=transferSplit(text,4,psize-1);
	rep=algo4(tab,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo4=" << rep << endl;
		getchar();
	}
	
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	tab=transferSplit(text,s,psize-1);
	rep=algo5(tab,pattern,s);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo5=" << rep << endl;
		getchar();
	}
	
	rep=algo6(tab,pattern,s);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo6=" << rep << endl;
		getchar();
	}
	
	return 0;
}