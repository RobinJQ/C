#include <stdio.h>
#include <stdlib.h>
#include "memory_testing.h"

double a[] = {1.0, 2.0, 3.0, 4.0, 5.0};
double b[] = {6.0, 7.0, 8.0, 9.0, 10.0};
double c[] = {11.0, 12.0, 13.0, 14.0, 15.0};
int n = sizeof(a) / sizeof (*a);

int main(int argc, char const *argv[])
{
	read_as_float(1077936128);
	restrict_arrays();
	restrict_assignment();
	return 0;
}

void restrict_assignment()
{
	printf("%s\n", "------ restrict_assignment ------");
	printf("%s\n", "Try to set two retrict pointers to same address on stack:");
	printf("%s%d\n","a = ",5);
	int a = 5;
	int* restrict ap = &a;
	int* restrict bp = ap;
	printf("%s%d\n","*ap = ",*ap);
	printf("%s%d\n","*bp = ",*bp);

	printf("%s\n", "Try to set two restrict pointers to same address on heap:");
	int* restrict p0 = calloc(1,sizeof(int));
	*p0 = 1;
	int* restrict p1;
	p1 = p0;

	printf("%s%d\n", "*p0 = ", *p0);
	printf("%s%d\n", "*p1 = ", *p1);

	printf("%s\n", "---------------------------------");
}

void read_as_float(int a) 
{
	printf("%s\n", "------ read_as_float ------");
	float b = *(float*)&a;
	printf("b = %f\n", b);
	printf("%s\n\n", "---------------------------");
}

void restrict_arrays() {
	printf("%s\n\n", "------ restrict_arrays ------");
	non_overlapping_arrays();
	overlapping_arrays();
	completely_overlapping_arrays();
	printf("%s\n\n", "-----------------------------");
}

void overlapping_arrays() 
{
	printf("%s\n", "------ overlapping arrays ------");
	sum_of_restrict_arrays(n-2,a,a+1,a+2);
	sum_of_arrays(n-2,a,a+1,a+2);
	printf("%s\n\n", "-----------------------------");
}

void non_overlapping_arrays() 
{
	printf("%s\n", "------ non overlapping arrays ------");
	sum_of_restrict_arrays(n,a,b,c);
	sum_of_arrays(n,a,b,c);
	printf("%s\n\n", "-----------------------------");
}

void completely_overlapping_arrays() 
{
	printf("%s\n", "------ completely overlapping arrays ------");
	sum_of_restrict_arrays(n,a,a,a);
	sum_of_arrays(n,a,a,a);
	printf("%s\n\n", "-----------------------------");
}

void sum_of_restrict_arrays(size_t n,
								 double* restrict a,
								 double* restrict b,
								 double* restrict c)
{
	printf("%s\n", "(restrict)");
	for (int i = 0; i < n; ++i)
	{
		printf("%s\n", "(before)");
		print_arrays(a,b,c,i);
		a[i] = b[i] + c[i];
		printf("%s\n", "(after)");
		print_arrays(a,b,c,i);
	}
}

void sum_of_arrays(size_t n,
					 double* a,
					 double* b,
					 double* c)
{
	printf("%s\n", "(non restrict)");
	for (int i = 0; i < n; ++i)
	{
		printf("%s\n", "(before)");
		print_arrays(a,b,c,i);
		a[i] = b[i] + c[i];
		printf("%s\n", "(after)");
		print_arrays(a,b,c,i);
	}
}

void print_arrays(double* a,
				  double* b, 
				  double* c,
				  int i)
{
	printf("%s%d%s%.1f\n","a[",i,"] = ",a[i]);
	printf("%s%d%s%.1f\n","b[",i,"] = ",b[i]);
	printf("%s%d%s%.1f\n","c[",i,"] = ",c[i]);
	printf("\n");
}

