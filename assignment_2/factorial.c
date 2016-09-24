//factorial by atf08rqv
//Robin Qvarfordt 890601-4157

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	if (argv[1] == NULL)
	{
		printf("No number entered\n");
		exit(1);
	}
	int n = atoi(argv[1]);
	size_t size = n*(floor(log10(abs(n)))+1);
	int res[size];
	res[0] = 1;
	int last = 1;

	for (int i = 1; i <= n; ++i)
	{
		int temp = 0;

		for (int j = 0; j < last; ++j)
		{
			temp += res[j] * i;
			res[j] = temp % 10; 
			temp /= 10;
		}
		while (temp >= 1)
		{
			res[last++] = temp % 10;
			temp /= 10;
		}

	}

	for (int i = last-1; i >= 0; i--)
	{
		printf("%d",res[i]);
	}
	printf("\n");

	return 0;
}