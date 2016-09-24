#include <stdio.h>
#include "poly.h"

static void poly_test(char* a, char* b)
{
poly_t* p;
poly_t* q;
poly_t* r;
printf("Begin polynomial test of (%s) * (%s)\n", a, b);
p = new_poly_from_string(a);
q = new_poly_from_string(b);
print_poly(p);
print_poly(q);
r = mul(p, q);
print_poly(r);
free_poly(p);
free_poly(q);
free_poly(r);
printf("End polynomial test of (%s) * (%s)\n\n\n", a, b);
}
int main(void)
{
poly_test("x^2 - 7x + 1", "3x + 2");
poly_test("x^10000000 + 2", "2x^2 + 3x + 4");
return 0;
}

/*
The output from this program should be:
Begin polynomial test of (x^2 - 7x + 1) * (3x + 2)
x^2 - 7 x + 1
3 x + 2
3 x^3 - 19 x^2 - 11 x + 2
End polynomial test of (x^2 - 7x + 1) * (3x + 2)
Begin polynomial test of (x^10000000 + 2) * (2x^2 + 3x + 4)
x^10000000 + 2
2 x^2 + 3 x + 4
2 x^10000002 + 3 x^10000001 + 4 x^10000000 + 4 x^2 + 6 x + 8
End polynomial test of (x^10000000 + 2) * (2x^2 + 3x + 4)
*/