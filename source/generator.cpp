#include <iostream>
#include <time.h>
#include <intrin.h>
#include <string.h>
#include <vector>
#include <fstream>
using namespace std;
char first,last;
int Tsize,Psize,repeats,minrep;

char getRandChar()
{
	return (rand()%(last-first))+first;
}

char *getRandString(int size)
{
	char *tab=new char[size+1];
	for(int i=0;i<size;i++)
	{
		tab[i]=getRandChar();
	}
	tab[size]=0;
	return tab;
}

int countRepeats(string *txt,string *pattern)
{
	int rep=0,pos=0;
	while((pos=txt->find(*pattern,pos))!=string::npos)
	{
		rep++;
		pos++;
	}
	return rep;
}

char *generatepattern(char *text, int size)
{
	string txt=text;
	string pattern="";

	for(int s=1;s<=size;s++)
	{
		pattern.resize(size,first);
		int curr,max=0;
		char best=first;
		for(char c=first;c<=last;c++)
		{
			pattern[s-1]=c;
			curr=countRepeats(&txt,&pattern);
			if(curr>max)
			{
				max=curr;
				best=c;
			}
		}
		pattern[s-1]=best;
	}
	
	repeats=countRepeats(&txt,&pattern);
	char *pat=new char[size+1];
	strcpy(pat,pattern.c_str());
	return pat;
}

int main(int argc,char **argv)
{
	srand(time(0));

	first='A';
	last='C';
	Tsize=5000000;
	Psize=18;
	minrep=1;
	
	char* text=getRandString(Tsize);
	char* pattern=generatepattern(text,Psize);

	do{
		text=getRandString(Tsize);
		pattern=generatepattern(text,Psize);
		cout << "repeats generated: " << repeats << endl;
	}while(repeats<minrep);

	string fname="";
	fname+=first;
	fname+=last;
	fname+="-";

	char buf[10];
	fname+=itoa(repeats,buf,10);
	fname+="-";
	fname+=pattern;

	ofstream file;
	file.open(fname.c_str(),ios::trunc);
	file << text << endl;
	file << pattern << endl;
	file << repeats << endl;
	file.close();
	system("pause");
	return 0;
}