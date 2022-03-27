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
    - set one matrix
*/


void randMatrix(int* matrix, int r, int c);
void printMatrix(int* matrix, int r, int c);
void printFMatrix(float* matrix, int r, int c);

void readMatrixF(char* path, int* matrix, int r, int c);
void writeMatrixF(char* path, int* matrix, int r, int c);
void zeroMatrix(int* matrix, int r, int c);
void oneMatrix(int* matrix, int r, int c);
void zeroFMatrix(float* matrix, int r, int c);
void oneFMatrix(float* matrix, int r, int c);

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
            printf("%d\t", matrix[i * r + j]);
        // jump
        printf("\n");
    } 
}

void printFMatrix(float* matrix, int r, int c){
    // access in row
    for (int i = 0; i < r; i++) {
        // access in columns
        for (int j = 0; j < c; j++)
            printf("%.2f\t", matrix[i * r + j]);
        // jump
        printf("\n");
    }
}

void readMatrixF(char* name, int* matrix, int r, int c){
    /*
    Given a matrix row as 0 0 0 0 0\n
    */
    FILE * fptr;
    int i, j, data;
    
    fptr = fopen(name, "r");
    if(fptr == NULL){
        printf("Error!, that file does not exists in the current path\n");
        exit(1);
    }

    for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            fscanf(fptr, "%d ", &data);
            matrix[i * r + j] = data;
        }
        fscanf(fptr, "\n", NULL);
    }

    fclose(fptr);
}

void writeMatrixF(char* name, int* matrix, int r, int c){
    FILE* fptr;
    int i, j;

    fptr = fopen(name, "w");

    for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            fprintf(fptr, "%d ", matrix[i * r + j]);
        }
        fprintf(fptr, "\n", NULL);
    }

    fclose(fptr);
}

void zeroMatrix(int* matrix, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j=0; j< c; j++){
            matrix[i * r + j] = 0;
        }
    }
}

void oneMatrix(int* matrix, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j=0; j< c; j++){
            matrix[i * r + j] = 1;
        }
    }
}

void zeroFMatrix(float* matrix, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j=0; j< c; j++){
            matrix[i * r + j] = 0.0;
        }
    }
}

void oneFMatrix(float* matrix, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j=0; j< c; j++){
            matrix[i * r + j] = 1.0;
        }
    }
}