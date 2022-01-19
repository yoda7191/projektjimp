#ifndef WIELOMIANBAZA4_H_
#define WIELOMIANBAZA4_H_

#include <stdio.h>
#include "points.h"

int wielomianBaza4();

// typedef struct {
// 		int n;
// 		double *x;
// 		double *f;
// 		double *f1;
// 		double *f2;
// 		double *f3;
//         double *f4;
// } poly_t;

typedef struct {
    int n;
    double *x;
    double *y;
    double *f;
//    double *f1;
//    double *f2;
//    double *f3;
//    double *f4;
} poly_t;

int alloc_poly (poly_t * poly, int n);
int read_poly ( FILE * inf, poly_t * poly);
void write_poly (poly_t * poly, FILE * ouf);
double value_poly (poly_t * poly, double x);
void make_poly (poly_t * poly, points_t * pts);

#endif

// #ifndef SPLINES_H
// #define SPLINES_H

// #include <stdio.h>

// typedef struct {
// 		int n;
// 		double *x;
// 		double *f;
// 		double *f1;
// 		double *f2;
// 		double *f3;
// } spline_t;

// int alloc_spl( spline_t *spl, int n );

// int  read_spl ( FILE *inf,  spline_t *spl );

// void  write_spl ( spline_t *spl, FILE * ouf );

// double value_spl( spline_t *spl, double x);

// #endif
