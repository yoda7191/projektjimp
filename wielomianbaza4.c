#include "makespl.h"
#include "gaus/piv_ge_solver.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>


double fx(double a[], double x)
{
	return a[0] + a[1]*x + a[2]*x*x + a[3]*x*x*x + a[4]*x*x*x*x;
}	

double d1f(double a[], double x)
{
	return a[1] + 2*a[2]*x + 3*a[3]*x*x + 4*a[4]*x*x*x;
}

double d2f(double a[], double x)
{	
	return 2*( a[2] + 3*a[3]*x + 6*a[4]*x*x ); 
}

double d3f(double a[], double x)
{
	return 6*( a[3] + 4*a[4]*x );
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

void make_spl(points_t * pts, spline_t * spl)	
{
	matrix_t *eqs = NULL;		// to będzie macierz wspolczynnikow 
	
	double *x = pts->x;		// tablica x-ow
	double *y = pts->y;		// tablica y-ow
	int n = pts->n;
	int i, j, k;
	double p;

	eqs = make_matrix(5, 6);	// 5x5 - wymimiar macierzy wspolczynnikow
	int nb = 5;			// 5x1 - wymiar macierzy prawych stron
	
	for( i = 0; i < n; i++ ) {	// wypelniamy macierz
		// sprytniejsze rozwiązanie
		for( j=0; j < 5; j++ ) {
		       for( k=0; k < 5; k++ ) {
			        p = (double)(j+k); 
		       		add_to_entry_matrix(eqs, j, k, 2*pow(x[i], p));	
		       }
		       p = (double)j;
		       add_to_entry_matrix(eqs, j, 5, 2*y[i]*pow(x[i], p));
		}
	}

 #ifdef DEBUG
	write_matrix(eqs, stdout);
 #endif

	if(piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}

 #ifdef DEBUG
	write_matrix(eqs, stdout);
 #endif

	double a[5]; 	// wspolczynniki poszukiwanego wielomianu 4 stopnia 
	if(alloc_spl(spl, n) == 0) {
		for( k = 0; k < nb; k++) 
			a[k] = get_entry_matrix(eqs, k, nb);
		for( i=0; i < spl->n; i++) {
			double xx = spl->x[i] = x[i];

			spl->f[i] = fx(a, xx);
			spl->f1[i] = d1f(a, xx);
		        spl->f2[i] = d2f(a, xx);
			spl->f3[i] = d3f(a, xx);	
		}
	}

 #ifdef DEBUG 
	printf("%f %f %f %f %f\n", a[0], a[1], a[2], a[3], a[4]);
 #endif
}
