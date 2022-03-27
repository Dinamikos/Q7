#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
This lib contains all of the functions that are important
while you are programming matrix operations.

    - random matrix
    - read matrix from file
    - write matrix > file
    - print matix
    - set zero matrix
*/


void randMatrix(int* matrix, int r, int c);
void printMatrix(int* matrix, int r, int c);

void readMatrixF(char* path, int* matrix, int r, int c);
void writeMatrixF(char* path, int* matrix, int r, int c);
void zeroMatrix(int* matrix, int r, int c);



void random_matrix(int* matrix, int r, int c){
    int j, i;
    srand(time(NULL));
    /* Putting 1 to 12 in the 1D array in a sequence */
    for (int i = 0; i < r; i++){
        for (int j=0; j< c; j++){
            matrix[i * c + j] = rand()%10;
        }
    }
}

void printMatrix(int* matrix, int r, int c){
    // access in row
    for (int i = 0; i < r; i++) {
        // access in columns
        for (int j = 0; j < c; j++)
            printf("%d ", matrix[i * c + j]);
        // jump
        printf("\n");
    } 
}

void readMatrixF(char* path, int* matrix, int r, int c){

}

void writeMatrixF(char* path, int* matrix, int r, int c){

}

void zeroMatrix(int* matrix, int r, int c){

}