#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*
This is a lib with different applications for matrix multiplication
based in different strategies below sequential and parallel application,
sequential/parallel function nomeclature:
    S/P_methodMM/VMM
*/
void S_orgMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c);

void P_rowMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter);
void P_colMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter);
void P_blockMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int bsize, int thread_counter);

void S_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c);
void P_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c, int thread_counter);




void S_orgMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c){
    int i, j, k;
    //set in zero
    zeroMatrix(matrix_C, r, c);
	// Multiplying first Matrix and second Matrix and storing in result_mat.
	for(i = 0; i < r; ++i)	{
		for(j = 0; j < c; ++j){
			for(k=0; k<r; ++k){
				matrix_C[i * r + j] += matrix_A[i * r + k] * matrix_B[k * r + j];
			}
		}
	}
}

void P_rowMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter){
    int i, j, k;
    //set in zero
    zeroMatrix(matrix_C, r, c);
	// Multiplying first Matrix and second Matrix and storing in result_mat.
    #pragma omp parallel for num_threads(thread_counter) private(j,k) shared(matrix_A, matrix_B, matrix_C)
	for(i = 0; i < r; ++i)	{
		for(j = 0; j < c; ++j){
			for(k=0; k<r; ++k){
				matrix_C[i * r + j] += matrix_A[i * r + k] * matrix_B[k * r + j];
			}
		}
	}
}

void P_colMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter){
    int i, j, k;
    //set in zero
    zeroMatrix(matrix_C, r, c);
    // Multiplying first Matrix and second Matrix and storing in result_mat.
    #pragma omp parallel for num_threads(thread_counter) private(j,k) shared(matrix_A, matrix_B, matrix_C)
	for(i = 0; i < r; ++i)	{
		for(j = 0; j < c; ++j){
			for(k=0; k<r; ++k){
				matrix_C[j * r + i] += matrix_A[j * r + k] * matrix_B[k * r + i];
			}
		}
	}
}

void P_blockMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int bsize, int thread_counter){
	int i, j, k, jj, kk;
    double temp;
    int N = bsize * (r/bsize);

    //set in zero
    zeroMatrix(matrix_C, r, c);

    for(jj = 0;jj < N; jj += bsize){
  
        for(kk = 0;kk < N; kk += bsize){
            #pragma omp parallel for num_threads(thread_counter) private(j,k,temp) shared(matrix_C, jj, kk, matrix_A, matrix_C)
            for(i = 0 ;i < N; i++){
                for(j = jj; j < ((jj+bsize)>N ? N:(jj+bsize)); j++){
                    temp = 0;
                    for(k = kk; k < ((kk+bsize)>N ? N:(kk+bsize)); k++){ //[i * N + k] [k * N +j]
                        temp += matrix_A[i * N + k] * matrix_B[k * N + j];
                    }
                    matrix_C[i * N + j] += temp;
                }
            }
        }
  }
}

void S_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c){
    int i, j, tn, w, cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);

    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover -= (float)matrix_A[i * r + j] * matrix_R[j];
            } else {
                break;
            } 
        }
        matrix_R[i] = (float)cover/w;
    }
}

void P_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c, int thread_counter){
    int i, j, tn, w, cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);
    #pragma omp parallel for num_threads(thread_counter) private(tn, w, cover, j) shared(matrix_A, matrix_R, matrix_T)
    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover -= (float)matrix_A[i * r + j] * matrix_R[j];
            } else {
                break;
            } 
        }
        matrix_R[i] = (float)cover/w;
    }
}