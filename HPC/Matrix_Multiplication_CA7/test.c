#include "mtxutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void S_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c){
    int i, j, tn, w;
    float cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);

    //printVectorFloatMatrix(matrix_R, r);

    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        
        w = matrix_A[i * r + i];
        cover = tn;


        for (j = 0; j < r; j++){
            if (i != j){
                cover =  cover - (float)matrix_A[i * c + j] * matrix_R[j];
                //printf("%f\n", (float)matrix_A[i * c + j] * matrix_R[j]);
            } else {
                break;
            } 
        }
        //printf("%f\n", cover);
        matrix_R[i] = (float)cover/w;
    }
}

void P_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c, int thread_counter){
    int i, j, tn, w;
    float cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);
    //printVectorFloatMatrix(matrix_R, r);
    #pragma omp parallel for num_threads(thread_counter) private(tn, w, cover, j) shared(matrix_A, matrix_R, matrix_T)

    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover = cover - ((float)matrix_A[i * r + j] * matrix_R[j]);
            } else {
                break;
            } 
        }
        matrix_R[i] = (float)cover/w;
    }
}

void P_triangularVMMS(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c, int thread_counter){
    int i, j, tn, w;
    float cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);
    //printVectorFloatMatrix(matrix_R, r);
    #pragma omp parallel for schedule(static) num_threads(thread_counter) private(tn, w, cover, j) shared(matrix_A, matrix_R, matrix_T)

    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover = cover - ((float)matrix_A[i * r + j] * matrix_R[j]);
            } else {
                break;
            } 
        }
        matrix_R[i] = (float)cover/w;
    }
}

void P_triangularVMMD(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c, int thread_counter){
    int i, j, tn, w;
    float cover;

    //set in one
    oneFMatrix(matrix_R, r, 1);
    //printVectorFloatMatrix(matrix_R, r);
    #pragma omp parallel for schedule(dynamic) num_threads(thread_counter) private(tn, w, cover, j) shared(matrix_A, matrix_R, matrix_T)

    for (i = 0; i < r; i++){
        tn = matrix_T[i];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover = cover - ((float)matrix_A[i * r + j] * matrix_R[j]);
            } else {
                break;
            } 
        }
        matrix_R[i] = (float)cover/w;
    }
}

void main(int argc, char** argv){

  //Time

    double timeSpent_s, timeSpent_p, time_total, speedup;
    clock_t begin_s, begin_p;
    clock_t end_s, end_p;
  //
    int r = 2500, c = 2500;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixB = malloc(r * sizeof(int));
    float* matrixC = malloc(r * sizeof(float));
    //randMatrix(matrixA,r,c);
    //writeMatrixF("newT.txt",matrixA,r,1);
    readMatrixF("new.txt", matrixA,r,c);
    readVectorMatrixF("newT.txt", matrixB,r);
    //printVectorMatrix(matrixB, r);
    //printMatrix(matrixB, r, c);
    //randMatrix(matrixA, r, c);
    //writeMatrixF("matrix2.txt", matrixA, r, 1);
    //S_triangularVMM(matrixA, matrixB, matrixC, r, c);
    //printVectorFloatMatrix(matrixC, r);
    //random_matrix(Matrix1, r,c);
    //random_matrix(Matrix2, r, c);
    //random_matrix(result_mat, r, c);

    //printf("Matrix 1:\n");
    //print_mat(Matrix1, r, c);
    //printf("Matrix 2:\n");
    //print_mat(Matrix2, r, c);
    //printf("Result:\n");
    
    int thread_counter = strtol(argv[1],NULL,10);
    oneFMatrix(matrixC, r, 1);

    //Sequencial
    begin_s = clock();

    S_triangularVMM(matrixA, matrixB, matrixC, r, c);
    
    end_s = clock();
    timeSpent_s = (double)(end_s - begin_s) / CLOCKS_PER_SEC;
    //printVectorFloatMatrix(matrixC, r);
    //printf("\nwapa: %f\n", timeSpent_s);
    //writeMatrixF("hola",matrixC, r,1);
    //Parallel
    timeSpent_p = 0.0;
    begin_p = clock();
    P_triangularVMMD(matrixA, matrixB, matrixC, r, c, thread_counter);

    end_p = clock();
    timeSpent_p += (double)(end_p - begin_p) / CLOCKS_PER_SEC;
    //printf("\nwapasar: %f\n", timeSpent_p);

    //print_mat(result_mat, r, c); 
    speedup= timeSpent_s/ timeSpent_p;
    //printf("Running time: %f seconds with %d threads\n", timeSpent_p, thread_counter);
    printf("\n%d: %f\n", thread_counter, speedup);
}
/*

1: 0.984127
2: 1.145161
3: 1.490196
4: 1.083333
5: 1.000000
6: 1.258065
7: 0.784810
8: 1.807692



VMMS
1: 1.238095
2: 0.937500
3: 0.984127
4: 0.576923
5: 0.984127
6: 1.032258
7: 1.238095
8: 1.274194
[1.238095,0.937500,0.984127,0.576923,0.984127,1.032258,1.238095,1.274194]


VMMD
1: 0.659574
2: 0.784810
3: 1.000000
4: 0.666667
5: 0.659574
6: 0.794872
7: 0.807692
8: 0.807692
[0.659574, 0.784810, 1.000000, 0.666667, 0.659574, 0.794872, 0.807692, 0.807692]
*/