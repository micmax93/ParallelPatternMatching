#include <iostream>
#include <cstdio>
#include <conio.h>
#include <omp.h>
#include "str.h"
#include <intrin.h>

using namespace std;

#pragma intrinsic(__rdtsc)


int algoSeq(char *text,char *pattern)
{
	int n=0;
	char *tptr,*pptr;
#pragma omp single
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

int algoParaAll(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for schedule(static, 1) num_threads(4) shared(text, pattern) reduction(+:n)
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

int algoParaFastAll(char *text,char *pattern,int tsize) // najlepszy z rodziny 2
{
	int n=0;

	#pragma omp parallel for schedule(static, 2048) num_threads(4) shared(text, pattern) reduction(+:n)
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

int algoParaTab(char *text,char *pattern,int tsize) // najlepszy z rodziny 3
{
	int n=0;

	#pragma omp parallel for schedule(static,1) shared(text, pattern) reduction(+:n)
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

int algoParaThreadDiv(char **ttab,char *pattern) // najlepszy z rodziny 4
{
	int s=4;
	int n=0;

	#pragma omp parallel for schedule(static, 1) reduction(+:n)
	for(int i=0;i<s;i++)
	{
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
			text++;
		}
	}

	return n;
}

int algoParaCacheDiv(char **ttab,char *pattern,int s) // najlepszy z rodziny 6
{
	int n=0;

	#pragma omp parallel for num_threads(4) schedule(static, 1) shared(pattern)  reduction(+:n)
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
		
		//#pragma omp atomic
		n+=subsum;
	}
	return n;
}

int algoSeq2(char *text,char *pattern)
{
	return algoSeq(text,pattern);
}

int main(int argc,char **argv)
{
	char *text,*pattern;
	loadInstance("AM-9-EAAAA",&text,&pattern);
	int rep;
	int tsize=strSize(text);
	int psize=strSize(pattern);
	char **cacheDiv,**threadDiv;

	threadDiv=transferSplit(text,4,psize-1);
	int s=calcSplitN(tsize,60*1024,psize-1,true);
	cacheDiv=transferSplit(text,s,psize-1);
	for(int i=0; i<1; ++i)
	{
		rep = algoParaAll(text,pattern,tsize);
		rep = algoParaFastAll(text,pattern,tsize);
		rep = algoParaTab(text,pattern,tsize);
		rep = algoSeq2(text,pattern);
		rep = algoParaThreadDiv(threadDiv,pattern);
		rep = algoParaCacheDiv(cacheDiv,pattern,s);
	}
	return 0;
}































































































































/**
int algo2(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for schedule(static,2048)
	for(int i=0;i<tsize;i++) // schedule(static,1)
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
int algo21(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for schedule(static,8) num_threads(4)
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

	#pragma omp parallel for schedule(static,1)
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
int algo5(char **ttab,char *pattern,int s)
{
	int n=0;
	#pragma omp parallel for schedule(static,1)
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
int algo51(char **ttab,char *pattern,int s) // najlepszy z rodziny 5
{
	int n=0;
	#pragma omp parallel for schedule(static,1) shared(pattern) reduction(+:n)
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
		
		//#pragma omp atomic
		n+=subsum;
	}

	return n;
}
int algo4(char **ttab,char *pattern)
{
	int s=4;
	int n=0;

	#pragma omp parallel for schedule(static,1) shared(pattern) reduction(+:n)
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
*/