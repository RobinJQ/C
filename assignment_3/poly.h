#include "stdio.h"

#ifndef poly_h
#define poly_h

typedef struct poly_t poly_t;

poly_t* init_poly(size_t map_size);
poly_t* new_poly_from_string(const char*);
void free_poly(poly_t*);
poly_t* mul(poly_t* p1, poly_t* p2);
void print_poly(poly_t*);

#endif