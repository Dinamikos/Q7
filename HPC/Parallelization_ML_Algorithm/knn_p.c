#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

void readDatasetFromFile(char * path, int n, int dataset[n][3]);

int main(){
    int rank, size, n, k;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        // MAIN PROCESS
        // printf("Enter the number of data points: ");
        // fflush(stdout);
        // scanf("%d", &n);

        // printf("Enter the number of nearest neighbors: ");
        // fflush(stdout);
        // scanf("%d", &k);

        // int dataset[n][3];
        //read the dataset
        // readDatasetFromFile("dataset2.txt", n, dataset);

        //divide the dataset into chunks
        n = 21;
        int aux = 0;
        float chunk_size = (int)floor((float)n / size);
        printf("\nChunk size: %f\n", chunk_size);
        for (int i = 0; i < size; i++){
            //getting te start
            // int index1 = aux;
            
            // //getting the end
            // float index = n/
            // MPI_Send(&times, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        

    } else {
        // SLAVE PROCESS

    }
    
    MPI_Finalize();

    return 0;
}


void readDatasetFromFile(char * path, int n, int dataset[n][3]){
    FILE * fp;
    fp = fopen(path, "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    
    int i;
    for(i = 0; i < n; i++){
        fscanf(fp, "%d %d %d", &dataset[i][0], &dataset[i][1], &dataset[i][2]);
    }
    
    fclose(fp);
}
