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

void S_triangularVMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c);
void P_triangularVMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter);




void S_orgMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c){

}

void P_rowMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter){

}

void P_colMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter){

}

void P_blockMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int bsize, int thread_counter){

}

void S_triangularVMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c){

}

void P_triangularVMM(int* matrix_A, int* matrix_B, int* matrix_C, int r, int c, int thread_counter){

}