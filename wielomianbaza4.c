#include "gaus/matrix.h"
#include "wielomianbaza4.h"
#include "gaus/piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>

#define MALLOC_FAILED( P, SIZE ) (((P)=malloc( (SIZE)*sizeof( *(P))))==NULL)
    
    /*
    Macierz A współczynników przy a0, a1, …, a4:
    | 2*i	         2*Ex           2*E(x^2)    2*E(x^3)     2*E(x^4) |
    | 2*Ex	         2*E(x^2)    2*E(x^3)     2*E(x^4)    2*E(x^5) |
    | 2*E(x^2)    2*E(x^3)     2*E(x^4)    2*E(x^5)    2*E(x^6) |
    | 2*E(x^3)     2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7) |
    | 2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7)     2*E(x^8) |
    make_matrix(i, i); - odwolujemy sie do matrix.c, ktore tworzy macierz
    */   

/**
 * M/ wiesz co, chyba niepotrzebnie to robiłeś XD
 * nasz program ma mieć strukture make_spl( points_t *pts, spline_t *spl)
 * i chyba musimy bazować właśnie na strukturze spl a używać tylko jednej funkcji
 * a z resztą się w praniu sprawdzu
 */
double fx(double a0, double a1, double a2, double a3, double a4, double x)
{
	return a0 + a1*x + a2*x*x + a3*x*x*x + a4*x*x*x*x;
}	

int alloc_poly (poly_t * poly, int n)
{
    poly->n = n;
    return MALLOC_FAILED (poly->x, poly->n)
        || MALLOC_FAILED (poly->f, poly->n);
//        || MALLOC_FAILED (poly->f1, poly->n)
//        || MALLOC_FAILED (poly->f2, poly->n)
//        || MALLOC_FAILED (poly->f3, poly->n)
//        || MALLOC_FAILED (poly->f4, poly->n);
}

int read_poly ( FILE * inf, poly_t * poly)
{
    if ( fscanf (inf, "%d", &(poly->n)) != 1 || poly->n < 0)
        return 1;

    if ( alloc_poly (poly, poly->n))
        return 1;

    for (int i = 0; i < poly->n; i++)
        if (fscanf(inf, "%lf %lf", poly->x + i, poly->f + i) != 2)
            return 1;

    return 0;  
}

void write_poly (poly_t * poly, FILE * ouf)
{
    fprintf (ouf, "%d\n", poly->n);
    for (int i = 0; i < poly->n; i++)
        fprintf (ouf, "%g %g\n", poly->x[i], poly->f[i]);
}

// tutaj trzeba to przerobic na nasza aproksymacje chora

// M/ aproksymacje mają mieć strukture make_spl( points_t * pts, splines_t *spl)
// musimy przyjmować wskaźnik na strukture punktów (tam tablice double x i y) 
// i spline_t 


double value_poly (poly_t * poly, double x)
{
    int i = 0;
    double dx;
    for (i = poly->n; i > 0; i--)
        if (poly->x[i] < x)
            break;
    
    dx = x - poly->x[i];

    //return poly->f[i] + dx * poly->f1[i] + dx * dx / 2 *  poly->f2[i] + dx * dx * dx / 6 * poly->f3[i];
    return 0;
}
/**
 * M/
E – suma elementów od 0 do n-1, gdzie n – ilość punktów pomiarowych
Macierz A współczynników przy a0, a1, …, a4:
| 2*n         2*Ex        2*E(x^2)    2*E(x^3)    2*E(x^4) |
| 2*Ex        2*E(x^2)    2*E(x^3)    2*E(x^4)    2*E(x^5) |
| 2*E(x^2)    2*E(x^3)    2*E(x^4)    2*E(x^5)    2*E(x^6) |
| 2*E(x^3)    2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7) |
| 2*E(x^4)    2*E(x^5)    2*E(x^6)    2*E(x^7)    2*E(x^8) |

Macierz X:
| a0 |
| a1 |
| a2 |
| a3 |
| a4 |

Macierz wyrazów wolnych (macierz B prawych stron):
| 2*Eyn        |
| 2*E(xi*yi)   |
| 2*E(xi^2*yi) |
| 2*E(xi^3*yi) |
| 2*E(xi^4*yi) |
**/
void make_poly(poly_t * poly, points_t * pts)
	
{
	printf("Zaczynam make_poly\n");

	matrix_t *eqs = NULL;		// to będzie macierz wspolczynnikow 
	//poly_t *poly = NULL;		// czy poly w main?
	double *x = pts->x;		// tablica x-ow
	double *y = pts->y;		// tablica y-ow
	int n = pts->n;
	int i, j, k;
	
	printf("x = ");
	for( i = 0; i < n; i++ ) 
		printf("%f\n",x[i]);
	printf("\ny = \n");
	for( i = 0; i < n; i++)
		printf("%f\n", y[i]);

	printf("n = %d\n", n);

	eqs = make_matrix(5, 6);	// 5x5 - wymimiar macierzy wspolczynnikow
	int nb = 5;			// 5x1 - wymiar macierzy prawych stron
	
	printf("Obliczam elemety macierzy z x i y\n");

	add_to_entry_matrix(eqs, 0, 0, 2*n);
	for( i = 0; i < n; i++ ) {	// wyliczamy wsółczynniki
		// spróbować zrobić to sprytniej
		printf("Jestem w pętli, element %d\n", i);
		add_to_entry_matrix(eqs, 0, 1, 2*x[i]);
		add_to_entry_matrix(eqs, 0, 2, 2*x[i]*x[i]);
		add_to_entry_matrix(eqs, 0, 3, 2*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 0, 4, 2*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 1, 0, 2*x[i]);
		add_to_entry_matrix(eqs, 1, 1, 2*x[i]*x[i]);
		add_to_entry_matrix(eqs, 1, 2, 2*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 1, 3, 2*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 1, 4, 2*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 0, 2*x[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 1, 2*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 2, 2*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 3, 2*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 4, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 0, 2*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 1, 2*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 2, 2*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 3, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 4, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 0, 2*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 1, 2*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 2, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 3, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 4, 2*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]*x[i]);
		
		add_to_entry_matrix(eqs, 0, 5, 2*y[i]);	
		add_to_entry_matrix(eqs, 1, 5, 2*y[i]*x[i]);
		add_to_entry_matrix(eqs, 2, 5, 2*y[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 3, 5, 2*y[i]*x[i]*x[i]*x[i]);
		add_to_entry_matrix(eqs, 4, 5, 2*y[i]*x[i]*x[i]*x[i]*x[i]);
	}
	
	printf("Obliczone (oby)\n");
// #ifdef DEBUG
	//write_matrix(eqs, stdout);
// #endif
	printf("Wyliczamy macierz (piv_ge_solver)\n");

	if(piv_ge_solver(eqs)) {
		poly->n = 0;
		return;
	}

// #ifdef DEBUG
	//write_matrix(eqs, stdout);
// #endif
	printf("Obliczamy współczynniki wielomianu (oby):\n");
	double a[5]; // niewiadomwe 
	if(alloc_poly(poly, n) == 0) {
		printf("pętla z a[k]\n");
		for( k = 0; k < nb; k++) {
			a[k] = get_entry_matrix(eqs, k, nb);
		}
		for( i=0; i < poly->n; i++) {
			printf("Obliczamy wartości funkcji dla x i zapisujemy w poly\n");
			double xx = poly->x[i] = x[i];

			poly->f[i] = fx(a[0], a[1], a[2], a[3], a[4], xx);
		}
	}

// #ifdef DEBUG 
	printf("%f %f %f %f %f\n", a[0], a[1], a[2], a[3], a[4]);
// #endif
}
