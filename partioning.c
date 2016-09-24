#include <stdio.h>
#include <math.h>

long int phi(int k,int n);
long int p(int n);

int main(void)
{
	int n = 50;
	printf("%ld\n", phi(1,n));
	printf("%ld\n", p(n));
	return 0;
}

long int p(int n) {
	return exp(M_PI*sqrt(2.0*n/3.0))/(4.0*n*sqrt(3.0));
}

long int phi(int k, int n) {
	if (n == 0)	{
		//printf("\n");
		return 1;
	} 
	if (n < k) {
		//printf("%s", "(X)");
		return 0;
	} 
	long int sum = 0;
	for (int t = 0; t*k <= n; ++t)
	{	
		for (int i = 0; i < t; ++i)
		{
			//printf("%d", k);
		}
		sum += phi(k+1,n-t*k);
	}
	return sum;
}