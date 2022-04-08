#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

void readDatasetFromFile(char * path, int n, int dataset[n][3]);
double eucledianDistance(int x1, int y1, int x2, int y2);

int main(){
    int rank, size, n, k;
    int new_data[3];
    int hola[5];


    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        hola[0] = 1;
        hola[1] = 1;
        hola[2] = 2;
        hola[3] = 3;
        hola[4] = 4;
        MPI_Bcast(hola, 5, MPI_INT, 0, MPI_COMM_WORLD);

    } else {
        MPI_Bcast(hola, 5, MPI_INT, 0, MPI_COMM_WORLD);
        printf("%d\n", hola[rank]);
    }
    
    MPI_Finalize();

    return 0;
}