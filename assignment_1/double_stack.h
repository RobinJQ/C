#include <stdio.h>

#ifndef DOUBLE_STACK_H
#define DOUBLE_STACK_H

typedef struct {
  double* doubles;
  int top;
  int maxSize;
}double_stack;

void push(double value,double_stack* stack);
double pop(double_stack* stack);
double_stack* init_double_stack(size_t maxSize);
void free_double_stack();
size_t size_of_stack(double_stack* stack);

#endif