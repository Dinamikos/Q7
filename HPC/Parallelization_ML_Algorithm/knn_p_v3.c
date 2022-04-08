#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

/**
 * Advanced Parallel K-Nearest Neighbors with MPI
 * next challenge:
 * - use omp parallel for to parallelize the computation
 * - use MPI clause to distribute the computation
 */

void readDatasetFromFile(char * path, int n, int dataset[n][3]);
double eucledianDistance(int x1, int y1, int x2, int y2);
void Od_even_sort(int size, double a[size][2], int thread_count);
void makePrediction(int k, double neighbors[k][2]);

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
        //read the dataset
        int dataset[n][3];
        readDatasetFromFile("dataset.txt", n, dataset);

        //send n and dataset to all other processes
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&dataset, n*3, MPI_INT, 0, MPI_COMM_WORLD);

        //set the euclidean distances
        double eucledian_distances[n][2];

        //set new data
        int new_data[2] = {48, 142000};
        // MPI_Bcast(&new_data, 2, MPI_INT, 0, MPI_COMM_WORLD);

        int chunk_size = (int)round((float)n/size);
        int aux = 0;
        //send dataset to all other processes
        for (int i = 1; i < size; i++){
            indexStart = aux;
            // MPI_Send(&n, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(new_data, 2, MPI_INT, i, i, MPI_COMM_WORLD);
            // MPI_Send(dataset, n*3, MPI_INT, i, i, MPI_COMM_WORLD);
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
        
        for (int d = 0, i = 1; i < size; i++){
            //get the size of the chunk
            MPI_Recv(&size_chunk, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double chunk[size_chunk][2];
            // printf("\nProcess %d: %d\n", i, size_chunk);
            MPI_Recv(chunk, size_chunk*2, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j = 0; j < size_chunk; j++){
                eucledian_distances[d][0] = chunk[j][0];
                eucledian_distances[d][1] = chunk[j][1];
                // printf("\nProcess %d: %f %f\n", i, eucledian_distances[d][0], eucledian_distances[d][1]);
                d++;
                // printf("%f\n", chunk[j]);
            }
            
        }

        // order the eucledian distances
        Od_even_sort(n, eucledian_distances, size);

        // print the eucledian distances sorted
        // for (int i = 0; i < n; i++){
        //     printf("%f - %f\n",eucledian_distances[i][0] ,eucledian_distances[i][1]);
        // }

        //make the prediction
        makePrediction(k, eucledian_distances);

    } else {
        // SLAVE PROCESS
        //receive n from main process
        // MPI_Recv(&n, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //receive new data from main process
        int new_data[2];
        MPI_Recv(new_data, 2, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Bcast(&new_data, 2, MPI_INT, 0, MPI_COMM_WORLD);


        //receive dataset from main process
        int dataset[n][3];
        // MPI_Recv(dataset, n*3, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Bcast(&dataset, n*3, MPI_INT, 0, MPI_COMM_WORLD);
        

        //get the start and end of the dataset
        MPI_Recv(&indexStart, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&indexEnd, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int size_chunk = indexEnd - indexStart;
        double distances[size_chunk][2];
        // printf("from process %d\n", rank);

        for (int j=0,i = indexStart; i < indexEnd; i++, j++){
            //correct label
            distances[j][0] = dataset[i][2];
            //eucledian distance for the label
            distances[j][1] = eucledianDistance(dataset[i][0], dataset[i][1], new_data[0], new_data[1]);
            // printf("%f - %f\n", distances[j][0], distances[j][1]);

        }
        // printf("\n");

        //send the size of the chunk to merge
        MPI_Send(&size_chunk, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        //send the distances to merge
        MPI_Send(distances, size_chunk*2, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
        
    }
    
    MPI_Finalize();

    return 0;
}
void makePrediction(int k, double neighbors[k][2]){


    int fr[k];
    int visited = -1;

    for(int i = 0; i <k; i++){
        int count = 1;
        for(int j = i+1; j < k; j++){
            if(neighbors[i][0] == neighbors[j][0]){
                count++;
                //To avoid counting same element again
                fr[j] = visited;
            }
        }
        if(fr[i] != visited){
            fr[i] = count;
        }
    }

    // get the element with the max frequency
    int max_frequency = 0;
    int max_frequency_index = 0;
    for (int i = 0; i < k; i++){
        if(fr[i] != visited){
            if(fr[i] > max_frequency){
                max_frequency = fr[i];
                max_frequency_index = i;
            }
        }
    }
    printf("\nPrediction: %d\n", (int)neighbors[max_frequency_index][0]);
}

void Od_even_sort(int size, double a[size][2], int thread_count){
    int phase, i;
    double temp,templ; 
    /**
     * 
     * a[size][2]
     * 
     * a[size][0] = label
     * a[size][1] = eucledian distance
     */
    for(phase = 0; phase < size; phase++){
        if (phase%2== 0) {
            //#pragma omp parallel for num_threads(thread_count) default(none) shared(a,size) private (i, temp)
            for (i=1; i< size; i+= 2){
                if (a[i-1][1]> a[i][1]){
                    temp= a[i][1];
                    templ= a[i][0];
                    a[i][1]= a[i-1][1];
                    a[i][0]= a[i-1][0];
                    a[i-1][1]= temp;
                    a[i-1][0]= templ;
                }
            }
        }
        else {
            //#pragma omp parallel for num_threads(thread_count) default(none) shared(a,size) private(i, temp)
            for (i=1; i< size-1; i+=2){
                if (a[i][1]> a[i+1][1]){
                    temp= a[i][1];
                    templ= a[i][0];
                    a[i][1]= a[i+1][1];
                    a[i][0]= a[i+1][0];
                    a[i+1][1]= temp;
                    a[i+1][0]= templ;
                }
            }
        }
    }
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
