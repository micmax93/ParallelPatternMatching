#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;


///�aduje tekst i wz�r z pliku
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

///Oblicza ilo�� znak�w cstr zako�czonego NULL
int strSize(char *str)
{
	int n=0;
	while(str[n]!=0)
		{n++;}
	return n;
}

///Rozdziela cstr 'source' na 'n' cz�ci z uwzgl�dnieniem
///nak�adki o rozmiarze 'copy' oraz z NULL na ko�cu
char **transferSplit(char *source,int n,int copy=0)
{
	char **tab=(char**)malloc(n*sizeof(char*));

	int size=strSize(source);
	int part=ceil((double)(size+(n-1)*copy)/(double)(n));

	int startp=0;
	for(int i=0;i<n;i++)
	{
		tab[i]=new char[part+1];
		for(int j=0;j<part;j++)
		{
			tab[i][j]=source[j+startp];
		}
		tab[i][part]=0;
		startp+=part-copy;
	}

	return tab;
}

///Pozwala na obliczenie na ile cz�ci nale�y podzieli�
///tekst o rozmiarze 'in' aby cz�ci zmie�ci�y si�
///w pakietach o rozmiarze 'out' z uwzgl�dnieniem nak�adki
///o rozmiarze okre�lonym w 'copy' oraz zako�czonego NULL
///co jest okre�lone przez 'nullEnd'.
int calcSplitN(int in,int out,int copy=0,bool nullEnd=false)
{
	if(nullEnd) {out--;}
	double n;
	n=in-copy;
	n/=out-copy;
	return ceil(n);
}

///Pozwala na obliczenie rozmiaru cz�ci na kt�re zosta�
///podzielony tekst o rozmiarze 'in' aby otrzyma� 'n' cz�ci
///przy czym uwzgl�dnieniamy nak�adk� o rozmiarze okre�lonym
///w 'copy' oraz zako�czonego NULL je�eli 'nullEnd'=true.
int calcSplitSize(int in,int n,int copy=0,bool nullEnd=false)
{
	int size=ceil((double)(in+(n-1)*copy)/(double)(n));
	if(nullEnd) {size++;}
	return size;
}