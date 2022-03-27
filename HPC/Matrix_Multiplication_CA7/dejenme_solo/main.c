#include "mtxutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void S_triangularVMM(int* matrix_A, int* matrix_T, float* matrix_R, int r, int c){
    int i, j, tn, w, cover;

    for (i = 0; i < r; i++){
        tn = matrix_T[i * r + 0];
        w = matrix_A[i * r + i];
        cover = tn;

        //printf("%d\n", tn);

        for (j = 0; j < r; j++){
            if (i != j){
                cover -= (float)matrix_A[i * r + j] * matrix_R[j * r + 0];
            } else {
                break;
            } 
        }

        matrix_R[i * r + 0] = (float)cover/w;
        
    }
    
}


int main(){
    int r = 3, c = 3;
    int* matrixA = malloc((r * c) * sizeof(int));
    int* matrixT = malloc((r * 1) * sizeof(int));
    float* matrixR = malloc((r * 1) * sizeof(float));

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
    matrixT[0 * r + 0] = 2;
    matrixT[1 * r + 0] = -2;
    matrixT[2 * r + 0] = 10;

    // matrixR one
    oneFMatrix(matrixR, r, 1);

    S_triangularVMM(matrixA, matrixT, matrixR, r, c);
    printFMatrix(matrixR, r, 1);

    return 1;
}