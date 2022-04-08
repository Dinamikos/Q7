#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

void readDatasetFromFile(char * path, int n, int dataset[n][3], int n_threads);
double eucledianDistance(int x1, int y1, int x2, int y2);
void Od_even_sort(int size, double a[size][2], int n_threads);
void makePrediction(int k, double neighbors[k][2], int n_threads);

int main(){
    int rank, size, n, k;
    int indexStart = 0;
    int indexEnd = 0;
    int n_threads = 8;
    //Time
    double timeSpent_s, timeSpent_p, time_total, speedup;
    clock_t begin_s, begin_p;
    clock_t end_s, end_p;

    // #pragma omp parallel
    // int U = omp_get_num_procs();

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0){

        timeSpent_s = 0.0;
        begin_s = clock();
        // MAIN PROCESS
        n = 210;
        //read the dataset
        int dataset[n][3];
        readDatasetFromFile("datasetISC.csv", n, dataset, n_threads);

        //send n and dataset to all other processes
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&dataset, n*3, MPI_INT, 0, MPI_COMM_WORLD);

        //set the euclidean distances
        double eucledian_distances[n][2];

        //set new data
        int new_data[2] = {20, 20};

        int chunk_size = (int)round((float)n/size);
        int aux = 0;


        //send index to all other processes
        #pragma omp parallel for reduction(+:aux) num_threads(n_threads) private(indexStart, indexEnd)
        {
        for (int i = 1; i < size; i++){
            indexStart = aux;
            MPI_Send(new_data, 2, MPI_INT, i, i, MPI_COMM_WORLD);

            //if it is the last process, send the last index
            indexEnd = i == size-1 ? n : indexStart + chunk_size;

            aux = indexEnd;
            //send the start and end of the chunk
            MPI_Send(&indexStart, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(&indexEnd, 1, MPI_INT, i, i, MPI_COMM_WORLD);

        }
        }

        int size_chunk;
        
        #pragma omp parallel for num_threads(size-1) private(size_chunk)
        {
        for (int d = 0, i = 1; i < size; i++){
            //get the size of the chunk
            MPI_Recv(&size_chunk, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //receive the chunk
            double chunk[size_chunk][2];
            MPI_Recv(chunk, size_chunk*2, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //save the results in eucledian_distances
            for (int j = 0; j < size_chunk; j++){
                eucledian_distances[d][0] = chunk[j][0];
                eucledian_distances[d][1] = chunk[j][1];
                d++;
            }
            
        }
        }

        // order the eucledian distances
        Od_even_sort(n, eucledian_distances, n_threads);

        //make the prediction
        makePrediction(k, eucledian_distances, n_threads);

        end_s = clock();
        timeSpent_s += (double)(end_s - begin_s) / CLOCKS_PER_SEC;
        printf("\nSequential Time: %f\n", timeSpent_s);

    } else {
        // SLAVE PROCESS
        //receive n from main process
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //receive new data from main process
        int new_data[2];
        MPI_Recv(new_data, 2, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //receive dataset from main process
        int dataset[n][3];
        MPI_Bcast(&dataset, n*3, MPI_INT, 0, MPI_COMM_WORLD);
        
        //get the start and end of the chunk
        MPI_Recv(&indexStart, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&indexEnd, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int size_chunk = indexEnd - indexStart;
        double distances[size_chunk][2];

        #pragma omp parallel for num_threads(2) 
        {
        for (int j=0,i = indexStart; i < indexEnd; i++, j++){
            //correct label
            distances[j][0] = dataset[i][2];
            //eucledian distance for the label
            distances[j][1] = eucledianDistance(dataset[i][0], dataset[i][1], new_data[0], new_data[1]);

        }
        }

        //send the size of the chunk to merge
        MPI_Send(&size_chunk, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        //send the distances to merge
        MPI_Send(distances, size_chunk*2, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
        
    }
    
    MPI_Finalize();

    return 0;
}
void makePrediction(int k, double neighbors[k][2], int n_threads){


    int fr[k];
    int visited = -1;

    #pragma omp parallel for num_threads(n_threads) shared(fr)
    {
    for(int i = 0; i <k; i++){
        int count = 1;
        for(int j = i+1; j < k; j++){
            if(neighbors[i][0] == neighbors[j][0]){
                count++;
                fr[j] = visited;
            }
        }
        if(fr[i] != visited){
            fr[i] = count;
        }
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

void Od_even_sort(int size, double a[size][2], int n_threads){
    int phase, i;
    double temp,templ;
    for(phase = 0; phase < size; phase++){
        if (phase%2== 0) {
            #pragma omp parallel for num_threads(n_threads) default(none) shared(a,size) private (temp, templ)
            {
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
        }
        else {
            #pragma omp parallel for num_threads(n_threads) default(none) shared(a,size) private(temp, templ)
            {
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
}

double eucledianDistance(int x1, int y1, int x2, int y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void readDatasetFromFile(char * path, int n, int dataset[n][3], int n_threads){
    FILE * fp;
    fp = fopen(path, "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    
    int i;
    for(i = 0; i < n; i++){
        fscanf(fp, "%d %d %d", &dataset[i][0], &dataset[i][1], &dataset[i][2]);
        // printf("%d %d %d\n", dataset[i][0], dataset[i][1], dataset[i][2]);
    }
    
    fclose(fp);
}
