//calculator by atf08rqv
//Robin Qvarfordt, 890601-4157

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "double_stack.h" 

#define INPUT_SIZE (20)

int main(int argc, char const *argv[])
{
	int c;
	bool isTypingNumber = false;
	double_stack* stack = init_double_stack(INPUT_SIZE);

	printf("Enter commands: \n");

	while ((c = getchar()) != EOF)
	{
		switch (c){

			case '\n':
				if (size_of_stack(stack) > 1)
				{
					fprintf(stderr, "Unused operands left\n");
					exit(1);
				}
				else if (size_of_stack(stack) == 1)
				{
					printf("Result: %.2lf\n",pop(stack));
				}
				isTypingNumber = false;
				break;

			case ' ':
				isTypingNumber = false;
				break;

			case '+':
				if (size_of_stack(stack) > 1)
				{
					double second = pop(stack);
					double first = pop(stack);
					push(first + second,stack);
					isTypingNumber = false;
				}
				else
				{
					//missplaced operator
					fprintf(stderr, "Error: Missplaced operator\n");
					exit(1);
				}
				break;

			case '-':
				if (size_of_stack(stack) > 1)
				{
					double second = pop(stack);
					double first = pop(stack);
					push(first - second,stack);
					isTypingNumber = false;
				}
				else
				{
					//missplaced operator
					fprintf(stderr, "Error: Missplaced operator\n");
					exit(1);
				}
				break;

			case '*':
				if (size_of_stack(stack) > 1)
				{
					double second = pop(stack);
					double first = pop(stack);
					push(first * second,stack);
					isTypingNumber = false;
				}
				else
				{
					//missplaced operator
					fprintf(stderr, "Error: Missplaced operator\n");
					exit(1);
				}
				break;

			case '/':
				if (size_of_stack(stack) > 1)
				{
					double second = pop(stack);
					if (abs(second) < pow(10,-10))
					{
						fprintf(stderr, "Error: Division by zero\n");
						exit(1);
					}
					double first = pop(stack);
					push(first / second,stack);
					isTypingNumber = false;
				}
				else
				{
					//missplaced operator
					fprintf(stderr, "Error: Missplaced operator\n");
					exit(1);
				}
				break;

			default:
				if (isdigit(c))
				{
					if (isTypingNumber)
					{
						double value = pop(stack);
						value = 10 * value + c - '0';
						push(value,stack);
					}
					else
					{
						push(c-'0',stack);
						isTypingNumber = true;
					}
				}
				else
				{
					//syntax error
					fprintf(stderr, "Error: Syntax error\n");
					exit(1);
				}
		}
	}
	free_double_stack(stack);
	return 0;
}
