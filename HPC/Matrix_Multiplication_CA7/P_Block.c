#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
void print_mat(int* Matrix1, int r, int c );
void random_matrix(int* Matrix1, int r, int c);

void random_matrix(int* Matrix, int r, int c){
  int j, i;
   /* Putting 1 to 12 in the 1D array in a sequence */
  srand(time(NULL));
  for (int i = 0; i < r; i++){
    for (int j=0; j< c; j++){
      Matrix[i * c + j] = rand()%10;
      }
  }
}

void print_mat(int* Matrix1, int r, int c ){
  for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf("%d ", Matrix1[i * c + j]);
        printf("\n");
    } 
}

void matrix_mult(int* Matrix1, int* Matrix2, int* result_mat, int r, int c, int bsize)
{
	int i, j, k, jj, kk;
  double temp;
  int N= bsize* (r/bsize);

	// Initialize elements of result matrix to 0.
  for (int i = 0; i < r; i++){
    for (int j=0; j< c; j++){
      result_mat[i * c + j] = 0;
      }
  }

  	// Multiplying first Matrix and second Matrix and storing in result_mat.
    
  for(int jj=0;jj<N; jj+= bsize){
    for(int kk=0; kk<N; kk+= bsize){
      for(int i=0;i<N;i++){
        for(int j = jj; j< ((jj+bsize)>N?N:(jj+bsize)); j++){
          temp = 0;
          for(int k = kk; k< ((kk+bsize)>N?N:(kk+bsize)); k++){ //[i * N + k] [k * N +j]
            temp += Matrix1[i * N + k]* Matrix2[k * N +j];
          }
          result_mat[i * N + j] += temp;
        }
      }
    }
  }

}

int main(int argc, char**argv){
  int r = 10, c = 10;
  int* Matrix1 = malloc((r * c) * sizeof(int));
  int* Matrix2 = malloc((r * c) * sizeof(int));
  int* result_mat = malloc((r * c) * sizeof(int));

  random_matrix(Matrix1, r,c);
  random_matrix(Matrix2, r, c);
  //random_matrix(result_mat, r, c);

  printf("Matrix 1:\n");
  print_mat(Matrix1, r, c);
  printf("Matrix 2:\n");
  print_mat(Matrix2, r, c);
  printf("Result:\n");
  matrix_mult(Matrix1, Matrix2, result_mat, r, c, 2);
  print_mat(result_mat, r, c); 
}