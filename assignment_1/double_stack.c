#include "double_stack.h"
#include <stdlib.h>

void push(double value, double_stack* stack)
{
	if (stack->top == stack->maxSize - 1)
	{
		fprintf(stderr, "Stack is full.\n");
    	exit(1);
	}
	stack -> doubles[++stack->top] = value;
}

double pop(double_stack* stack)
{
	if (stack->top == -1)
	{
		fprintf(stderr, "Stack is empty.\n");
    	exit(1);
	}
	return stack->doubles[stack->top--];
}

double_stack* init_double_stack(size_t maxSize)
{
	double_stack* stack = malloc(sizeof(double_stack));
	double* new_doubles = (double*) malloc(sizeof(double) * maxSize);
	if (new_doubles == NULL || stack == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
    	exit(1);
	}
	stack->doubles = new_doubles;
	stack->top = -1;
	stack->maxSize = maxSize;
	return stack;
}

void free_double_stack(double_stack* stack)
{
	free(stack->doubles);
	free(stack);
}
size_t size_of_stack(double_stack* stack)
{
	return stack->top + 1;
}