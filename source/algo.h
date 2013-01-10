#include <string.h>
#include <intrin.h>
#include <time.h>
unsigned long long start,stop;

///Przeszukuje tekst w poszukukiwaniu wzorca
bool checkPattern(char *text,char *pattern)
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

///Porównuje tekst z wzorcem
int countPatterns(char *text,char *pattern)
{
	int n=0;
	while(*text!=NULL)
	{
		if(*text==*pattern)
		{
			if(checkPattern(text+1,pattern+1))
				{n++;}
		}
		text++;
	}
	return n;
}

void startTimeCount()
{
	start=__rdtsc();
}

void stopTimeCount()
{
	stop=__rdtsc();
}

int getTime()
{
	double t=stop-start;
	t/=10000;
	return t;
}