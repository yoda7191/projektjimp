#ifndef WIELOMIANBAZA4_H_
#define WIELOMIANBAZA4_H_

#include <stdio.h>

int wielomianBaza4();

typedef struct {
		int n;
		double *x;
		double *f;
		double *f1;
		double *f2;
		double *f3;
//        double *f4;
} poly_t;

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