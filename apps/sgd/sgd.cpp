#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include"definitions.h"

// Read data files (as preprocessed by code)
#define X_matrix "X_ent.txt" //size MxN
#define X_v_matrix "X_valida.txt" //size MxN
#define b_vector "b_bh.txt" //size Nx1
#define y_vector "y_train.txt" //size Mx1
#define y_v_vector "y_val.txt" //size Mx1

void fill_matrix(array_2d_T p, const char *s){
    std::cout << "Top of fill matrix" << std::endl;
	int m = rows(p);
	int n = columns(p);
    std::cout << "Read m=" << m << ", n=" << n << std::endl;
	FILE * pFile;
  	pFile = fopen (s,"r");
  	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
            std::cout << "Reading entry i=" << i << ", j=" << j << std::endl;
			fscanf(pFile,"%lf", &value(p,i,j));
        }
    }
	fclose(pFile);
}

// FORTRAN (BLAS) function prototypes used for vector-matrix multiplication and scalar vector multiplication 
//extern void dgemv_(char *transpose_a, int *m, int *n, double *alpha, double *a, int *lda, double *x, int *incx, double *beta, double *y, int *incy);
//extern void daxpy_(int *n, double *a, double *x, int *incx, double *y, int *incy);
#include <cctype>

// C++ Implementation of BLAS DGEMV (Matrix-Vector Multiplication)
// Computes: y = alpha * A * x + beta * y   (or A^T * x)
// Note: Changed 'char*' to 'const char*' to permanently fix your previous string warning.
void dgemv_(const char *transpose_a, const int *m, const int *n, const double *alpha,
            const double *a, const int *lda, const double *x, const int *incx,
            const double *beta, double *y, const int *incy) {

    if (*m <= 0 || *n <= 0) return;

    // Check transposition character ('N' or 'n' for No Transpose)
    bool no_trans = (transpose_a[0] == 'N' || transpose_a[0] == 'n');
    int len_y = no_trans ? *m : *n;

    // 1. Scale vector y by beta
    if (*beta == 0.0) {
        int iy = (*incy < 0) ? (-len_y + 1) * (*incy) : 0;
        for (int i = 0; i < len_y; ++i) {
            y[iy] = 0.0;
            iy += *incy;
        }
    } else if (*beta != 1.0) {
        int iy = (*incy < 0) ? (-len_y + 1) * (*incy) : 0;
        for (int i = 0; i < len_y; ++i) {
            y[iy] *= *beta;
            iy += *incy;
        }
    }

    // If alpha is 0, the matrix multiplication clears out, so we are done
    if (*alpha == 0.0) return;

    // 2. Perform Matrix-Vector multiplication (Respecting Column-Major Storage)
    if (no_trans) {
        int jx = (*incx < 0) ? (-*n + 1) * (*incx) : 0;
        for (int j = 0; j < *n; ++j) {
            if (x[jx] != 0.0) {
                double temp = (*alpha) * x[jx];
                int iy = (*incy < 0) ? (-*m + 1) * (*incy) : 0;
                for (int i = 0; i < *m; ++i) {
                    y[iy] += temp * a[i + j * (*lda)]; // Column-major indexing
                    iy += *incy;
                }
            }
            jx += *incx;
        }
    } else { // Transpose case ("T" or "t")
        int jy = (*incy < 0) ? (-*n + 1) * (*incy) : 0;
        for (int j = 0; j < *n; ++j) {
            double temp = 0.0;
            int ix = (*incx < 0) ? (-*m + 1) * (*incx) : 0;
            for (int i = 0; i < *m; ++i) {
                temp += a[i + j * (*lda)] * x[ix]; // Column-major indexing
                ix += *incx;
            }
            y[jy] += (*alpha) * temp;
            jy += *incy;
        }
    }
}

// C++ Implementation of BLAS DAXPY (Scalar-Vector Addition)
// Computes: y = alpha * x + y
void daxpy_(const int *n, const double *alpha, const double *x, const int *incx,
            double *y, const int *incy) {

    if (*n <= 0 || *alpha == 0.0) return;

    // Handle standard positive or backward negative strides
    int ix = (*incx < 0) ? (-*n + 1) * (*incx) : 0;
    int iy = (*incy < 0) ? (-*n + 1) * (*incy) : 0;

    for (int i = 0; i < *n; ++i) {
        y[iy] += (*alpha) * x[ix];
        ix += *incx;
        iy += *incy;
    }
}

int main(int argc, char const *argv[]) {

    std::cout<< "Top of main" << std::endl;
  // Declaration of structures to store data
  array_2d_T X, X_v, batch;
	array_1d_T y, y_v, b, g, y_b, rmse, rmse_v;


  //Get data dimensions from standard input
  int M=3999805; //atoi(argv[1]);
  int N=39; //atoi(argv[2]);
  int M_v=1714203;//atoi(argv[3]);
  // Read batch size
  int batch_size=256;//atoi(argv[4]);
  // Read number of iterations
  int iter=200;//atoi(argv[5]);
  // Read learning rate value
  double lr=-0.001;//atof(argv[6]);

	int incx=1;
  double ALPHA, BETA;


    std::cout<< "Beginning malloc calls" << std::endl;
  // Assign memory space for out data structures
  X=(array_2d_T)malloc(sizeof(*X));
  X_v=(array_2d_T)malloc(sizeof(*X_v));
  y=(array_1d_T)malloc(sizeof(*y));
  y_v=(array_1d_T)malloc(sizeof(*y_v));
  b=(array_1d_T)malloc(sizeof(*b));
  g=(array_1d_T)malloc(sizeof(*g));
  batch=(array_2d_T)malloc(sizeof(*batch));
  y_b=(array_1d_T)malloc(sizeof(*y_b));
  rmse=(array_1d_T)malloc(sizeof(*rmse));
  rmse_v=(array_1d_T)malloc(sizeof(*rmse_v));
    std::cout<< "First block of malloc calls done" << std::endl;
// Assign dimension values for data structures
	rows(X)=M;
	columns(X)=N;
  rows(X_v)=M_v;
	columns(X_v)=N;
  rows_vector(b)=N;
	rows_vector(y)=M;
  rows_vector(y_v)=M_v;
  rows_vector(g)=N;
  rows(batch)=batch_size;
  columns(batch)=N;
  rows_vector(y_b)=batch_size;
  rows_vector(rmse)=iter;
  rows_vector(rmse_v)=iter;
    std::cout<< "Done with dimension assignments" << std::endl;

// Fill our structures with our data from .txt files
    std::cout<< "Malloc call for X matrix" << std::endl;
	values(X)=(double*)malloc(rows(X)*columns(X)*sizeof(double));
    std::cout<< "Filling the X matrix" << std::endl;
	fill_matrix(X,X_matrix);

    std::cout<< "Malloc call for X_v matrix" << std::endl;
  values(X_v)=(double*)malloc(rows(X_v)*columns(X_v)*sizeof(double));
    std::cout<< "Filling the X_v matrix" << std::endl;
  fill_matrix(X_v,X_v_matrix);

	values_vector(b)=(double*)malloc(N*sizeof(double));
	fill_vector(b,b_vector);

  values_vector(y)=(double*)malloc(M*sizeof(double));
  fill_vector(y,y_vector);

  values_vector(y_v)=(double*)malloc(M_v*sizeof(double));
  fill_vector(y_v,y_v_vector);

	values_vector(g)=(double*)malloc(N*sizeof(double));

  values(batch)=(double*)malloc(rows(batch)*columns(batch)*sizeof(double));

  values_vector(y_b)=(double*)malloc(batch_size*sizeof(double));

  values_vector(rmse)=(double*)malloc(iter*sizeof(double));
  values_vector(rmse_v)=(double*)malloc(iter*sizeof(double));
    std::cout<< "Done with malloc calls" << std::endl;

// Start  iterations
for(int it = 0; it < iter; it++){ 
// Re-fill our response variable vectors every iteration as they get modified in each one (Details below)
  fill_vector(y,y_vector);
  fill_vector(y_v,y_v_vector);
  
  // Fill batch matrix. This step is what makes this algorithm stochastic as it randomly chooses a fixed number of records to train on every iteration
  fill_batch(batch, X, y_b, y);

// Calculate prediction error: e = - X %*% b + y
// Because of how dgemv function was programmed, vector y is overwrittten by result e (which is why we re-fill the y vector every iteration)
  ALPHA = -1.0;
  BETA = 1.0;
	dgemv_("No transpose", &batch_size, &N, &ALPHA, values(batch), &batch_size, values_vector(b), &incx, &BETA, values_vector(y_b),&incx);
  dgemv_("No transpose", &M, &N, &ALPHA, values(X), &M, values_vector(b), &incx, &BETA, values_vector(y),&incx);
  dgemv_("No transpose", &M_v, &N, &ALPHA, values(X_v), &M_v, values_vector(b), &incx, &BETA, values_vector(y_v),&incx);

// Calculation of the training and validation errors
  double acum = 0;
  for(int i = 0; i < M; i++){
    acum += pow(values_vector(y)[i],2);
  }
  double acum_v = 0;
  for(int i = 0; i < M_v; i++){
    acum_v += pow(values_vector(y_v)[i],2);
  }
  value_vector(rmse,it) = acum/M;
  value_vector(rmse_v,it) = acum_v/M_v;
  printf("Iteration %d/%d RMSE train: %lf -- RMSE val: %lf \n", it+1, iter, value_vector(rmse,it), value_vector(rmse_v,it));
  printf("------------\n");
// Calculating the gradient (for descent direction): g = -X^t %*% e
  ALPHA = -1.0;
  BETA = 0.0;
  dgemv_("Transpose", &batch_size, &N, &ALPHA, values(batch), &batch_size, values_vector(y_b), &incx, &BETA, values_vector(g),&incx);
// now vector g holds the gradient's value

// Update coefficients: b = b - lr * g
  daxpy_(&N, &lr,values_vector(g), &incx, values_vector(b), &incx);
}

printf("----- Final Coefficients -----\n");
print_vector(b);

// Save RMSE train and validation scores for later analysis.
FILE *f = fopen("RMSE_SGD.txt", "w");
if (f == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}
  fprintf(f, "iteration,rmse_t,rmse_v\n");
  for(int i=0; i<iter; i++){
    fprintf(f, "%d,%f,%f\n", i, value_vector(rmse,i),value_vector(rmse_v,i));
  }
fclose(f);

// Free up memory allocated to structures
  free(values(X));
  free(X);
  free(values_vector(b));
  free(b);
  free(values_vector(y));
  free(y);
  free(values_vector(g));
  free(g);
  free(values(X_v));
  free(X_v);
  free(values_vector(y_v));
  free(y_v);
  free(values(batch));
  free(batch);
  free(values_vector(y_b));
  free(y_b);

	return 0;

}
