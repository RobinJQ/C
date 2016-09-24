#include "poly.h"
#include "map.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAP_SIZE (20)

struct poly_t {
	map_t* terms;
	m_key_t* exponents;
	size_t exp_len;
};

poly_t* init_poly(size_t map_size)
{
	poly_t* poly = calloc(sizeof(poly_t),1);
    if (poly == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}
	poly->terms = init_map_with_size(map_size);
	poly->exponents = calloc(sizeof(m_key_t),map_size);
	if (poly->exponents == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}
	poly->exp_len = 0;

	if (poly == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}
	if (poly->exponents == NULL)
	{
		fprintf(stderr, "Error: Out of memory\n");
		exit(1);
	}

	return poly;
}

poly_t* new_poly_from_string(const char* cp)
{
	poly_t* poly = init_poly(MAP_SIZE);

	m_value_t value = 1;
	m_key_t exponent = 0;
	int sign = 1;
	bool variable_typed = false;
	bool raise_typed = false;
	bool typing_value = false;
	int c;
	int i = 0;

	while(true)
	{
		c = cp[i++];

		if (c == '\0')
		{
			if (value != 0)
			{
				if (variable_typed && !raise_typed)
				{
					exponent = 1;
				}
				if (set_pair(exponent, sign * value, poly->terms) == NULL)
				{
					m_value_t* old_value = value_for_key(exponent, poly->terms);
					*old_value += sign * value;
				}
				else
				{
					poly->exponents[poly->exp_len++] = exponent;
				}
				
			}
			break;
		}

		if (c == 'x')
		{
			variable_typed = true;
			continue;
		}
		else if (c == ' ')
		{
			continue;
		}
		else if (c == '^')

		{
			if (!variable_typed)
			{
				fprintf(stderr, "Error: '^' has to be preceded by a variable\n");
				exit(1);
			}
			raise_typed = true;
			typing_value = false;
			continue;
		}

		switch (c){

			case '+':
				if (value != 0)
				{
					if (variable_typed && !raise_typed)
					{
						exponent = 1;
					}
					if (set_pair(exponent, sign * value, poly->terms) == NULL)
					{
						m_value_t* old_value = value_for_key(exponent, poly->terms);
						*old_value += sign * value;
					}
					else
					{
						poly->exponents[poly->exp_len++] = exponent;
					}
					value = 1;
					exponent = 0;
				}
				sign = 1;
				raise_typed = false;
				variable_typed = false;
				typing_value = false;
				break;

			case '-':
				if (value != 0)
				{
					if (variable_typed && !raise_typed)
					{
						exponent = 1;
					}
					if (set_pair(exponent, sign * value, poly->terms) == NULL)
					{
						m_value_t* old_value = value_for_key(exponent, poly->terms);
						*old_value += sign * value;
					}
					else
					{
						poly->exponents[poly->exp_len++] = exponent;
					}
					value = 1;
					exponent = 0;
				}
				sign = -1;
				raise_typed = false;
				variable_typed = false;
				typing_value = false;
				break;

			default:
				variable_typed = false;
				if (isdigit(c))
				{
					if (!raise_typed)
					{
					
						if (typing_value)
						{
						
							value = 10*value + c - '0';
					
						}
						else
						{
							value = c -'0';
							typing_value = true;
						}
						
					}
					else
					{
						exponent = 10*exponent + c - '0';
					}
				}

				else
				{
					fprintf(stderr, "Error: Syntax error\n");
					exit(1);
				}
		}
	}
	return poly;
}
void free_poly(poly_t* poly)
{
	if (poly != NULL)
	{
		free_map(poly->terms);
		free(poly->exponents);
		free(poly);
	}
}
poly_t* mul(poly_t* p1, poly_t* p2)
{
	poly_t* p3 = init_poly(MAP_SIZE);

	for (size_t i = 0; i < p1->exp_len; ++i)
	{
		for (size_t j = 0; j < p2->exp_len; ++j)
		{
			m_key_t e1 = p1->exponents[i];
			m_key_t e2 = p2->exponents[j];
			m_value_t* v1 = value_for_key(e1, p1->terms);
			m_value_t* v2 = value_for_key(e2, p2->terms);

			if (set_pair(e1+e2, (*v1)*(*v2), p3->terms) == NULL)
			{
				m_value_t* old_value = value_for_key(e1+e2, p3->terms);
				*old_value += (*v1)*(*v2);
			}
			else
			{
				p3->exponents[p3->exp_len++] = e1+e2;
			}
		}
	}
	return p3;
}
void print_poly(poly_t* poly)
{
	if (poly != NULL)
	{
		size_t size = 0;
		pair_t** sorted_terms = map_to_sorted_array(poly->terms, &size);
		if (sorted_terms != NULL)
		{
			printf("%.2lf x^%ld ", *get_value(sorted_terms[0]), *get_key(sorted_terms[0]));
			for (size_t i = 1; i < size; ++i)
			{
				pair_t* term = sorted_terms[i];
				char sign;
				if (*get_value(term) < 0)
				{
					sign = '-';
				}
				else
				{
					sign = '+';
				}
				printf("%c %.2lf x^%ld ", sign, fabs(*get_value(term)), *get_key(term));
			}
		}
		printf("\n");
	}
}