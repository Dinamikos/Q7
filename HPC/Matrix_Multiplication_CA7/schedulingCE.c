#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n=4;
int m=1;
void print_matrix(int n, int m[4][4]); 
void print_vector(int n, int v[4]); 
void matrix_x_vector(int n, int y[4], int x[4][4], int A[4][1]);
void random_matrix(int Matrix[n][n], int n);
void random_vector (int vector[n], int n);
void random_mxn( int result [n][m], int n, int m);

int main(int argc, char**argv)
{ 
  int thread_counter = strtol(argv[1],NULL,10);
	int n = 4;
  int m= 1;
  int vector[n];
  random_vector(vector, n);
  int Matrix1[n][n];
  random_matrix(Matrix1, n);
  int result_mat [n][m];
  random_mxn(result_mat, n, m);
	
  
  print_vector(n, vector);
	print_matrix(n, Matrix1);

	matrix_x_vector(n, vector, Matrix1, result_mat);
	printf("\n");
	return 0;

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

void random_mxn( int result [n][m], int n, int m){
  int i,j;
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      result[i][j] = 0;
      }
    
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      printf(" %d ",result[i][j]);
      }
    printf("\n");
  }
  }
  }

void random_vector (int vector[n], int n){
  int i;
  for (int i = 0; i < n ;i++) {
    vector[i] = rand()%10;
    }
}
void print_matrix(int n, int m[4][4])

{
	int i, j; 

	printf("\nMatrix Given\n");
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
      printf("%3i", m[i][j]);
		printf("\n");
	}

}

void print_vector(int n, int v[4])
{
	int i;

	printf("\nVector Given\n");
	for (i=0; i<n; i++)

		printf("%3i", v[i]);
	printf("\n");

}
void matrix_x_vector(int n, int y[4], int x[4][4], int A[4][1])

{
	int i, j;
	printf("\nResulted Matrix of [M]*[V]\n");
	for (i=0; i<n; i++)

	{
		for (j=0; j<n; j++)
		{
			A[j][0] += x[j][i] * y[i];

			//printf("%4i", A[j][0]);

		}
    //printf("\n");

	}
	printf("\n");
	for (i=0; i<n; i++)

		printf("%4i\n", A[i][0]);

}