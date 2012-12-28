#include <iostream>
#include <time.h>
#include <intrin.h>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;


bool checkpattern(char *text,char *pattern)
{
	while(*pattern!=NULL)
	{
		if(*text!=*pattern)
			{return false;}
		text++;
		pattern++;
	}
	return true;
}

int countpatterns(char *text,char *pattern)
{
	int n=0;
	while(*text!=NULL)
	{
		if(*text==*pattern)
		{
			if(checkpattern(text+1,pattern+1))
				{n++;}
		}
		text++;
	}
	return n;
}

void loadInstance(char *filename,char **text,char **pattern)
{
	ifstream file;
	file.open(filename);
	string buf;
	
	buf.clear();
	getline(file,buf);
	*text=new char[buf.size()+1];
	strcpy(*text,buf.c_str());

	buf.clear();
	getline(file,buf);
	*pattern=new char[buf.size()+1];
	strcpy(*pattern,buf.c_str());

	int rep;
	file >> rep;
	cout << "Expected count " << rep << endl;

	file.close();
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

	unsigned long long start,stop;

	start = __rdtsc();
	int n=countpatterns(text,pattern);
	stop = __rdtsc();
	cout << "Found " << n << " occurrences of pattern" << endl;

	system("pause");
	return 0;
}