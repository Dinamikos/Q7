#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


int n;

void matrix_mult(int Matrix1[n][n], int Matrix2[n][n], int result_mat[n][n], int n, int thread_counter)
{
	int i, j, k;

	// Initialize elements of result matrix to 0.
	for(i = 0; i < n; ++i)	{
		for(j = 0; j < n; ++j)		{
			result_mat[i][j] = 0;
		}
	}

	// Multiplying first Matrix and second Matrix and storing in result_mat.
  #pragma omp parallel for num_threads(thread_counter) private(j,k) shared(result_mat, Matrix1, Matrix2)
	for(i = 0; i < n; ++i)	{
		for(j = 0; j < n; ++j){
			for(k=0; k<n; ++k){
				result_mat[j][i] += Matrix1[j][k] * Matrix2[k][i];
			}
		}
	}
}


void random_matrix(int Matrix[n][n], int n){
  int i,j;
  srand(time(NULL)); //seed to get random num
  for(i = 0; i < n;i++) { //rows
    for(j = 0; j < n;j++) { //columns
      Matrix[i][j] = rand()%10; 
    }   
  }
}

void print_mat (int Matrix[n][n], int n){
  int i,j;
  for(i = 0; i < n;i++) {
     for(j = 0; j < n;j++) { 
       printf("\t%d ", Matrix[i][j]); 
     } 
     printf("\n");
   }
}
int main(int argc, char**argv){
  int r = 10, c = 10;
  n = 10;
  int Matrix2[n][n];
  int Matrix1[n][n];
  int result_mat[n][n];
  int thread_counter = strtol(argv[1],NULL,10);

  random_matrix(Matrix1,n);
  random_matrix(Matrix2, n);
  //random_matrix(result_mat, r, c);

  printf("Matrix 1:\n");
  print_mat(Matrix1, n);
  printf("Matrix 2:\n");
  print_mat(Matrix2, n);
  printf("Result:\n");
  matrix_mult(Matrix1, Matrix2, result_mat, n, thread_counter);
  print_mat(result_mat, n); 
}