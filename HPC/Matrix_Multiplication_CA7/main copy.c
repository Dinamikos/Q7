#include "mtxutils.h"
#include "mtxmop.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void Emain(){
    int r = 3, c = 3;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixT = malloc(r * sizeof(int));
    float* matrixR = malloc(r * sizeof(float));

    // matrixA manual asignation
    
    matrixA[0 * r + 0] = 1;
    matrixA[0 * r + 1] = 0;
    matrixA[0 * r + 2] = 0;

    matrixA[1 * r + 0] = 2;
    matrixA[1 * r + 1] = 3;
    matrixA[1 * r + 2] = 0;
    
    matrixA[2 * r + 0] = 4;
    matrixA[2 * r + 1] = 2;
    matrixA[2 * r + 2] = 5;

    // matrixT manual asignation
    matrixT[0] = 2;
    matrixT[1] = -2;
    matrixT[2] = 10;

    // matrixR one
    //oneFMatrix(matrixR, r, 1);

    P_triangularVMM(matrixA, matrixT, matrixR, r, c, 2);
    printVectorFloatMatrix(matrixR, r);

}
/*

1Speedup: 1.192308
2Speedup: 0.659574
3Speedup: 1.937500
4Speedup: 1.000000
5Speedup: 0.468750
6Speedup: 0.709677
7Speedup: 1.000000
8Speedup: 0.933333
*/

void main(int argc, char** argv){

  //Time

    double timeSpent_s, timeSpent_p, time_total, speedup;
    clock_t begin_s, begin_p;
    clock_t end_s, end_p;
  //
    int r = 100, c = 100;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixB = malloc((r * c) * sizeof(int));
    int* matrixC = malloc((r * c) * sizeof(int));
    readMatrixF("matrix1.txt", matrixA,r,c);
    readMatrixF("matrix1.txt", matrixB,r,c);
    //randMatrix(matrixA, r, c);
    //writeMatrixF("matrix2.txt", matrixA, r, 1);

    //random_matrix(Matrix1, r,c);
    //random_matrix(Matrix2, r, c);
    //random_matrix(result_mat, r, c);

    //printf("Matrix 1:\n");
    //print_mat(Matrix1, r, c);
    //printf("Matrix 2:\n");
    //print_mat(Matrix2, r, c);
    //printf("Result:\n");

    int thread_counter = strtol(argv[1],NULL,10);

    //Sequencial
    timeSpent_s = 0.0;
    begin_s = clock();
    S_orgMM(matrixA, matrixB, matrixC, r, c);
    end_s = clock();
    timeSpent_s += (double)(end_s - begin_s) / CLOCKS_PER_SEC;

    //Parallel
    timeSpent_p = 0.0;
    begin_p = clock();
    P_rowMM(matrixA, matrixB, matrixC, r, c, thread_counter);
    end_p = clock();
    timeSpent_p += (double)(end_p - begin_p) / CLOCKS_PER_SEC;

    //print_mat(result_mat, r, c); 
    speedup= timeSpent_s/ timeSpent_p;
    //printf("Running time: %f seconds with %d threads\n", timeSpent_p, thread_counter);
    printf("\nSpeedup: %f\n", speedup);
}



    /*

#include "mtxutils.h"
#include "mtxmop.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void main(){
    int r = 10, c = 10;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixT = malloc(r * sizeof(int));
    float* matrixR = malloc(r * sizeof(float));

    // matrixA manual asignation
    //randMatrix(matrixA, r, c);
    //printMatrix(matrixA, r, c);

    // matrixT manual asignation
    //randVectorMatrix(matrixT, r);
    //printVectorMatrix(matrixT, r);

    S_triangularVMM(matrixA, matrixT, matrixR, r, c);
    printVectorFloatMatrix(matrixR, r);

}

    /*


  //Time

  double timeSpent_s, timeSpent_p, time_total, speedup;
  clock_t begin_s, begin_p;
  clock_t end_s, end_p;
  //

  random_matrix(Matrix1, r,c);
  random_matrix(Matrix2, r, c);
  //random_matrix(result_mat, r, c);

  //printf("Matrix 1:\n");
  //print_mat(Matrix1, r, c);
  //printf("Matrix 2:\n");
  //print_mat(Matrix2, r, c);
  //printf("Result:\n");

  //Sequencial
  timeSpent_s = 0.0;
  begin_s = clock();
  matrix_mult_s(Matrix1, Matrix2, result_mat, r, c, 2, thread_counter);
  end_s = clock();
  timeSpent_s += (double)(end_s - begin_s) / CLOCKS_PER_SEC;

  //Parallel
  timeSpent_p = 0.0;
  begin_p = clock();
  matrix_mult(Matrix1, Matrix2, result_mat, r, c, 2, thread_counter);
  end_p = clock();
  timeSpent_p += (double)(end_p - begin_p) / CLOCKS_PER_SEC;

  //print_mat(result_mat, r, c); 
  speedup= timeSpent_s/ timeSpent_p;
  printf("Running time: %f seconds with %d threads\n", timeSpent_p, thread_counter);
  printf("\nSpeedup: %f\n", speedup);


int Emain(){
    int r = 4, c = 4;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixB = malloc((r * c) * sizeof(int));
    int* matrixC = malloc((r * c) * sizeof(int));
    readMatrixF("matrix2.txt", matrixA, r, c);
    readMatrixF("matrix1.txt", matrixB, r, c);
    randMatrix(matrixA, r, c);

    P_blockMM(matrixA, matrixB, matrixC, r, c, 2, 2);
    writeMatrixF("matrix3.txt", matrixC, r, c);
    //readMatrixF("matrix1.txt", matrixA, r, c);
    //printMatrix(matrixA, r, c);
}
    

































int Emain(){
    int r = 4, c = 4;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixB = malloc((r * c) * sizeof(int));
    int* matrixC = malloc((r * c) * sizeof(int));
    readMatrixF("matrix2.txt", matrixA, r, c);
    readMatrixF("matrix1.txt", matrixB, r, c);
    randMatrix(matrixA, r, c);

    P_blockMM(matrixA, matrixB, matrixC, r, c, 2, 2);
    writeMatrixF("matrix3.txt", matrixC, r, c);
    //readMatrixF("matrix1.txt", matrixA, r, c);
    //printMatrix(matrixA, r, c);
}
    */