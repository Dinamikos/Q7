#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;
void matrix_mult(int Matrix1[n][n], int Matrix2[n][n], int result_mat[n][n], int n, int bsize);

void random_matrix(int Matrix[n][n], int n){
  int i,j;
  srand(time(NULL)); //seed to get random num
  for(i = 0; i < n;i++) { //rows
    for(j = 0; j < n;j++) { //columns
      Matrix[i][j] = rand()%10; 
    }   
  }
}

void matrix_mult(int Matrix1[n][n], int Matrix2[n][n], int result_mat[n][n], int n, int bsize)
{
	int i, j, k, jj, kk;
  double temp;
  int N= bsize* (n/bsize);

	// Initialize elements of result matrix to 0.
	for(i = 0; i < n; ++i)	{
		for(j = 0; j < n; ++j)		{
			result_mat[i][j] = 0;
		}
	}

	// Multiplying first Matrix and second Matrix and storing in result_mat.
  for(int kk=0;kk<N; kk+= bsize){
    for(int jj=0; jj<N; jj+= bsize){
      for(int i=0;i<N;i++){
        for(int j = jj; j< (jj+bsize); j++){
          temp = 0;
          for(int k = kk; k< (kk+bsize); k++){
            temp += Matrix1[i][k]*Matrix2[k][j];
          }
          result_mat[i][j] += temp;
        }
      }
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

int main(int argc, char**argv) {

  n = atoi(argv[1]);
  
  int Matrix2[n][n];
  int Matrix1[n][n];
  int result_mat[n][n];
  
  random_matrix(Matrix1, n);
  random_matrix(Matrix2, n);
  
  printf("Matrix 1:\n[");
  print_mat(Matrix1, n);
  printf("]\n");
  printf("Matrix 2:\n[");
  print_mat(Matrix2, n);
  printf("]\n");
  printf("Result:\n[");
  matrix_mult(Matrix1, Matrix2, result_mat, n, 4);
  print_mat(result_mat, n);
  printf("]\n");
	return 0;
}