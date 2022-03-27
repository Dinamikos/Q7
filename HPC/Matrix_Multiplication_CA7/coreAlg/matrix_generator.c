#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
void print_mat(int* Matrix1, int r, int c );
void random_matrix(int* Matrix1, int r, int c);

void random_matrix(int* Matrix1, int r, int c){
   /* Putting 1 to 12 in the 1D array in a sequence */
    for (int i = 0; i < r * c; i++)
        Matrix1[i] = rand()%10;
}

void print_mat(int* Matrix1, int r, int c ){
  for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf("%d ", Matrix1[i * c + j]);
        printf("\n");
    } 
}

int main(int argc, char**argv){
  int r = 2500, c = 2500;
  int* Matrix1 = malloc((r * c) * sizeof(int));
  int* Matrix2 = malloc((r * c) * sizeof(int));

  random_matrix(Matrix1, r,c);
  random_matrix(Matrix2, r, c);

  printf("Matrix 1:\n");
  print_mat(Matrix1, r, c);
  printf("Matrix 2:\n");
  print_mat(Matrix2, r, c);

  
}