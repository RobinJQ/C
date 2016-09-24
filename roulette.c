#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* spinn();
void simulate_roulette(int n);
static char* previous="red";


int main(void)
{
	simulate_roulette(10000000);
}

char* spinn()
{
	int value = rand() % 36 + 1;
	if(value % 2 == 0)
	{
		return "red";
	}
	if(value==36)
	{
		return "green";
	}
	return "black";
}

void simulate_roulette(int n)
{
	int count=0;
	int maxcount=0;
	int nbrOfWins=0;
	for(int i=0;i<n;i++)
	{	
		char* result = spinn();
		if(strcmp(result,"black")==0)
		{
			++nbrOfWins;
		}
		if(strcmp(previous,result)!=0)
		{
		//	printf("\n");
			if(count>maxcount)
			{
				maxcount=count;
			}
			count=0;
		}
		++count;
		//printf("%s %d\n",result,++count);
		previous=result;
	}
	printf("\nmaxcount = %d\nNbr of wins = %d\n",maxcount,nbrOfWins);
}