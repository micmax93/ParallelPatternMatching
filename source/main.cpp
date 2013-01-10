#include <iostream>
#include <cstdio>
#include <conio.h>
#include <omp.h>
#include "str.h"

using namespace std;

#pragma intrinsic(__rdtsc)

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

int algo22(char *text,char *pattern,int tsize) // najlepszy z rodziny 2
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

int algo23(char *text,char *pattern,int tsize)
{
	int n=0;

	#pragma omp parallel for schedule(dynamic, 2048) num_threads(4)
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

int algo31(char *text,char *pattern,int tsize) // najlepszy z rodziny 3
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

int algo4(char **ttab,char *pattern)
{
	int s=4;
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

int algo41(char **ttab,char *pattern) // najlepszy z rodziny 4
{
	int s=4;
	int n=0;

	#pragma omp parallel for schedule(static, 1) shared(ttab, pattern) reduction(+:n)
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
	#pragma omp parallel for schedule(static,1) shared(ttab, pattern) reduction(+:n)
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


int algo61(char **ttab,char *pattern,int s) // najlepszy z rodziny 6
{
	int n=0;

	#pragma omp parallel for num_threads(4) schedule(static, 1) shared(ttab, pattern)  reduction(+:n)
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

int main(int argc,char **argv)
{
	char *text,*pattern;
	//loadInstance("AC-382-AAAAAAAAAAAABAAAAA",&text,&pattern);
	loadInstance("AM-9-EAAAA",&text,&pattern);
	int rep;
	int tsize=strSize(text);
	int psize=strSize(pattern);
	char **tab;
	unsigned __int64 counterStart; 
	unsigned __int64 counterStop; 
	unsigned __int64  algo1Avg = 0;
	unsigned __int64  algo2Avg = 0;
	unsigned __int64  algo21Avg = 0;
	unsigned __int64  algo22Avg = 0;
	unsigned __int64  algo23Avg = 0;
	unsigned __int64  algo3Avg = 0;
	unsigned __int64  algo31Avg = 0;
	unsigned __int64  algo4Avg = 0;
	unsigned __int64  algo41Avg = 0;
	unsigned __int64  algo5Avg = 0;
	unsigned __int64  algo51Avg = 0;
	unsigned __int64  algo6Avg = 0;
	unsigned __int64  algo61Avg = 0;

	int iter = 10;
	for(int i=0; i<iter; ++i) {

	counterStart = __rdtsc();
	rep=algo1(text,pattern);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo1=" << rep << endl;
		getchar();
	}
	algo1Avg +=  (counterStop - counterStart);
	
	counterStart = __rdtsc();
	rep=algo2(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo2=" << rep << endl;
		getchar();
	}
	algo2Avg +=  (counterStop - counterStart);
	

	counterStart = __rdtsc();
	rep=algo21(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo21=" << rep << " " << expected << endl;
		getchar();
	}
	algo21Avg +=  (counterStop - counterStart);
	
	
	counterStart = __rdtsc();
	rep=algo22(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo22=" << rep << endl;
		getchar();
	}
	algo22Avg +=  (counterStop - counterStart);
	
	
	counterStart = __rdtsc();
	rep=algo23(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo22=" << rep << endl;
		getchar();
	}
	algo23Avg +=  (counterStop - counterStart);
	
	
	counterStart = __rdtsc();
	rep=algo3(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo3=" << rep << endl;
		getchar();
	}
	algo3Avg +=  (counterStop - counterStart);
	

	counterStart = __rdtsc();
	rep=algo31(text,pattern,tsize);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo31=" << rep << endl;
		getchar();
	}
	algo31Avg +=  (counterStop - counterStart);
	
	
	
	counterStart = __rdtsc();
	tab=transferSplit(text,4,psize-1);
	counterStop = __rdtsc();
	rep=algo4(tab,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo4=" << rep << endl;
		getchar();
	}
	algo4Avg +=  (counterStop - counterStart);
	

	counterStart = __rdtsc();
	tab=transferSplit(text,4,psize-1);
	counterStop = __rdtsc();
	rep=algo41(tab,pattern);
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo41=" << rep << endl;
		getchar();
	}
	algo41Avg +=  (counterStop - counterStart);
	

	int s=calcSplitN(tsize,60*1024,psize-1,true);
	tab=transferSplit(text,s,psize-1);
	
	
	counterStart = __rdtsc();
	rep=algo5(tab,pattern,s);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo5=" << rep << endl;
		getchar();
	}
	algo5Avg +=  (counterStop - counterStart);
	

	counterStart = __rdtsc();
	rep=algo51(tab,pattern,s);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo51=" << rep << endl;
		getchar();
	}
	algo51Avg +=  (counterStop - counterStart);
	

	
	counterStart = __rdtsc();
	rep=algo6(tab,pattern,s);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo6=" << rep << endl;
		getchar();
	}
	algo6Avg +=  (counterStop - counterStart);
	
	
	counterStart = __rdtsc();
	rep=algo61(tab,pattern,s);
	counterStop = __rdtsc();
	if(rep!=expected)
	{
		cout << "Error!" << endl;
		cout << "algo6=" << rep << endl;
		getchar();
	}
	algo61Avg +=  (counterStop - counterStart);
	

	}
	int scale = 1000;
	if (algo1Avg > 0)
	cout << "ALGO1:  " << algo1Avg/scale <<  "\n";
	if (algo2Avg > 0)
		cout << "ALGO2:  " << algo2Avg/scale << "\n"; 
	if (algo21Avg > 0)
		cout << "ALGO21: " << algo21Avg/scale << "\n"; 
	if (algo22Avg > 0)
		cout << "ALGO22: " << algo22Avg/scale << "\n"; 
	if (algo23Avg > 0)
		cout << "ALGO23: " << algo23Avg/scale << "\n"; 
	if (algo3Avg > 0)
		cout << "ALGO3:  " << algo3Avg/scale << "\n"; 
	if (algo31Avg > 0)
		cout << "ALGO31: " << algo31Avg/scale << "\n"; 
	if (algo4Avg > 0)
		cout << "ALGO4:  " << algo4Avg/scale << "\n"; 
	if (algo41Avg > 0)
		cout << "ALGO41: " << algo41Avg/scale << "\n"; 
	if (algo5Avg > 0)
		cout << "ALGO5:  " << algo5Avg/scale << "\n"; 
	if (algo51Avg > 0)
		cout << "ALGO5:  " << algo51Avg/scale << "\n"; 
	if (algo6Avg > 0)
		cout << "ALGO6:  " << algo6Avg/scale << "\n"; 
	if (algo61Avg > 0)
		cout << "ALGO61: " << algo61Avg/scale << "\n"; 


	return 0;
}