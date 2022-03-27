#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void print_mat(int* Matrix1, int r, int c );
void random_matrix(int* Matrix1, int r, int c);


void matrix_mult(int Matrix1[n][n], int Matrix2[n][n], int result_mat[n][n], int n)
{
	int i, j, k;

	// Initialize elements of result matrix to 0.
	for(i = 0; i < n; ++i)	{
		for(j = 0; j < n; ++j)		{
			result_mat[i][j] = 0;
		}
	}

	// Multiplying first Matrix and second Matrix and storing in result_mat.
	for(i = 0; i < n; ++i)	{
		for(j = 0; j < n; ++j){
			for(k=0; k<n; ++k){
				result_mat[i][j] += Matrix1[i][k] * Matrix2[k][j];
			}
		}
	}
}


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

int main(int argc, char**argv){
  int r = 10, c = 10;
  int* Matrix1 = malloc((r * c) * sizeof(int));
  int* Matrix2 = malloc((r * c) * sizeof(int));
  int* result_mat = malloc((r * c) * sizeof(int));
  int thread_counter = strtol(argv[1],NULL,10);

  random_matrix(Matrix1, r,c);
  random_matrix(Matrix2, r, c);
  //random_matrix(result_mat, r, c);

  printf("Matrix 1:\n");
  print_mat(Matrix1, r, c);
  printf("Matrix 2:\n");
  print_mat(Matrix2, r, c);
  printf("Result:\n");
  matrix_mult(Matrix1, Matrix2, result_mat, r, c, 2, thread_counter);
  print_mat(result_mat, r, c); 
}