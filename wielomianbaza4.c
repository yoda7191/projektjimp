#include "gaus/matrix.h"
#include "wielomianbaza4.h"

#include <stdlib.h>

#define MALLOC_FAILED( P, SIZE ) (((P)=malloc( (SIZE)*sizeof( *(P))))==NULL)

int wielomianBaza4 ()
{
    /*
    Macierz A współczynników przy a0, a1, …, a4:
    | 2*i	         2*Ex           2*E(x^2)    2*E(x^3)     2*E(x^4) |
    | 2*Ex	         2*E(x^2)    2*E(x^3)     2*E(x^4)    2*E(x^5) |
    | 2*E(x^2)    2*E(x^3)     2*E(x^4)    2*E(x^5)    2*E(x^6) |
    | 2*E(x^3)     2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7) |
    | 2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7)     2*E(x^8) |
    make_matrix(i, i); - odwolujemy sie do matrix.c, ktore tworzy macierz
    */   
}

int alloc_poly (poly_t * poly, int n)
{
    poly->n = n;
    poly->n = n;
    return MALLOC_FAILED (poly->x, poly->n)
        || MALLOC_FAILED (poly->f, poly->n)
        || MALLOC_FAILED (poly->f1, poly->n)
        || MALLOC_FAILED (poly->f2, poly->n)
        || MALLOC_FAILED (poly->f3, poly->n);
//        || MALLOC_FAILED (poly->f4, poly->n);
}

int read_poly ( FILE * inf, poly_t * poly)
{
    if ( fscanf (inf, "%d", &(poly->n)) != 1 || poly->n < 0)
        return 1;

    if ( alloc_poly (poly, poly->n))
        return 1;

    for (int i = 0; i < poly->n; i++)
        if (fscanf(inf, "%lf %lf %lf %lf %lf", poly->x + i, poly->f + i, poly->f1 + i, poly->f2 + i, poly->f3 + i) != 5)
            return 1;

    return 0;  
}

void write_poly (poly_t * poly, FILE * ouf)
{
    fprintf (ouf, "%d\n", poly->n);
    for (int i = 0; i < poly->n; i++)
        fprintf (ouf, "%g %g %g %g %g\n", poly->x[i], poly->f[i], poly->f1[i],poly->f2[i], poly->f3[i]);
}

// tutaj trzeba to przerobic na nasza aproksymacje chora
double value_poly (poly_t * poly, double x)
{
    int i = 0;
    double dx;
    for (i = poly->n; i > 0; i--)
        if (poly->x[i] < x)
            break;
    
    dx = x - poly->x[i];

    return poly->f[i] + dx * poly->f1[i] + dx * dx / 2 *  poly->f2[i] + dx * dx * dx / 6 * poly->f3[i];
}



