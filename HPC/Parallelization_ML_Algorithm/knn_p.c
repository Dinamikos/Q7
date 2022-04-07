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
    int indexStart = 0;
    int indexEnd = 0;


    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        // MAIN PROCESS
        n = 11;
        //send n to all other processes

        //read the dataset
        int dataset[n][3];
        
        readDatasetFromFile("dataset.txt", n, dataset);

        //set the euclidean distances
        double eucledian_distances[n];

        //set new data
        int new_data[2] = {48, 142000};
        int chunk_size = (int)round((float)n/size);
        int aux = 0;
        //send dataset to all other processes
        for (int i = 1; i < size; i++){
            indexStart = aux;
            MPI_Send(&n, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(new_data, 2, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(dataset, n*3, MPI_INT, i, i, MPI_COMM_WORLD);
            if (i == size-1){
                indexEnd = n;
            } else {
                indexEnd = indexStart + chunk_size;
            }
            aux = indexEnd;
            //send the start and end of the chunk
            // printf("\nProcess %d: %d %d\n", i, indexStart, indexEnd);
            MPI_Send(&indexStart, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(&indexEnd, 1, MPI_INT, i, i, MPI_COMM_WORLD);

        }

        int size_chunk;
        
        for (int i = 1; i < size; i++){
            //get the size of the chunk
            MPI_Recv(&size_chunk, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double chunk[size_chunk];
            printf("\nProcess %d: %d\n", i, size_chunk);
            // MPI_Recv(chunk, size_chunk, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // for (int j = 0; j < size_chunk; j++){
            //     printf("AAAA%f\n", chunk[j]);
            // }
            
        }
        

        //receive the new data from the process with the euclidean distance closest to the new data
        // MPI_Bcast(eucledian_distances, n, MPI_INT, 0, MPI_COMM_WORLD);
        

    } else {
        // SLAVE PROCESS
        //receive n from main process
        MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        //receive new data from main process
        int new_data[2];
        MPI_Recv(new_data, 2, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //receive dataset from main process
        int dataset[n][3];
        MPI_Recv(dataset, n*3, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        

        //get the start and end of the dataset
        MPI_Recv(&indexStart, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&indexEnd, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int size_chunk = indexEnd - indexStart;
        double distances[size_chunk];
        printf("from process %d\n", rank);
        for (int i = indexStart; i < indexEnd; i++){
            distances[i] = eucledianDistance(dataset[i][0], dataset[i][1], new_data[0], new_data[1]);
            printf("%f\n", distances[i]);
        }
        printf("\n");

        //send the size of the chunk to merge
        MPI_Send(&size_chunk, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        //send the distances to merge
        MPI_Send(distances, size_chunk, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
        
    }
    
    MPI_Finalize();

    return 0;
}


double eucledianDistance(int x1, int y1, int x2, int y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
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
