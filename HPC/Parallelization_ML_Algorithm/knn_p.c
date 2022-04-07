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


    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        // MAIN PROCESS
        printf("Enter the number of data points: ");
        fflush(stdout);
        scanf("%d", &n);
        //send n to all other processes
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        //new data
        printf("Enter the data to predict: ");
        fflush(stdout);
        scanf("%d %d %d", new_data[0], new_data[1], new_data[2]);
        // send new_data to all other processes
        MPI_Bcast(new_data, 3, MPI_INT, 0, MPI_COMM_WORLD);

        printf("Enter the number of nearest neighbors: ");
        fflush(stdout);
        scanf("%d", &k);

        int dataset[n][3];
        double eucledian_distances[n];
        MPI_Bcast(dataset, n*3, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(eucledian_distances, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // read the dataset
        readDatasetFromFile("dataset.txt", n, dataset);
        //send dataset to all other processes
        //send eucledian_distances to all other processes


        //divide the dataset into chunks
        n = 21;
        int aux = 0;
        float chunk_size = (int)floor((float)n / size);
        int indexStart = 0;
        int indexEnd = 0;
        printf("\nChunk size: %f\n", chunk_size);
        for (int i = 0; i < size; i++){
            //getting te start
            indexStart = aux;

            
            //getting the end
            if (i == size - 1){
                indexEnd = n;
            } else {
                indexEnd = aux + chunk_size;
            }
            aux = indexEnd;


            //send start index
            MPI_Send(&indexStart, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            //send end index
            MPI_Send(&indexEnd, 1, MPI_INT, i, i+1, MPI_COMM_WORLD);

        }
        
        

    } else {
        // SLAVE PROCESS
        // receive start index
        MPI_Recv(&indexStart, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // receive end index
        MPI_Recv(&indexEnd, 1, MPI_INT, 0, rank+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //calculate the eucledian of the dataset chunk with parralelization
        for (int i = 0; i < n; i++){
            eucledian_distances[i] = eucledianDistance(new_data[0], new_data[1], dataset[i][0], dataset[i][1]);
        }

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
