#include "mtxutils.h"
#include "mtxmop.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


void main(){
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
    printFMatrix(matrixR, r, 1);

}

    /*
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