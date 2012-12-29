#include <string.h>
#include <intrin.h>
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

double getTime()
{
	double t=stop-start;
	t/=1000.0;
	return t;
}