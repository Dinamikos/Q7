#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void readDatasetFromFile(char * path, int n, int dataset[n][3]);
double eucledianDistance(int x1, int y1, int x2, int y2);
void printDataset(int n, int dataset[n][3]);
void printDatasetWithEuclideanDistance(int n, int dataset[n][3], double dataset_x[n]);
void getNearestNeighbors(int n, int dataset[n][3], double dataset_x[n], int k, int nearest_neighbors[k]);
void predict(int new_data[2], int k, int nearest_neighbors[k]);


int main(void){
    //Time
    double timeSpent_s, timeSpent_p, time_total, speedup;
    clock_t begin_s, begin_p;
    clock_t end_s, end_p;
    //


    int n = 11;
    int dataset[n][3];
    double dataset_x[n];
    int k = 5;


    timeSpent_s = 0.0;
    begin_s = clock();


    readDatasetFromFile("dataset.txt", n, dataset);
    //cleaning two last columns in dataset
    for(int i = 0; i < n; i++){
        dataset_x[i] = 0;
    }
    int new_data[3] = {48, 142000, 0};
    int nearest_neighbors[k];
    //calculating distances
    for(int i = 0; i < n; i++){
        dataset_x[i] = eucledianDistance(dataset[i][0], dataset[i][1], new_data[0], new_data[1]);
    }
    
    //printing dataset and distances
    // printDatasetWithEuclideanDistance(n, dataset, dataset_x);

    //getting nearest neighbors
    getNearestNeighbors(n, dataset, dataset_x, k, nearest_neighbors);

    //printing nearest neighbors  
    printf("\nNearest neighbors:\n");
    for(int i = 0; i < k; i++){
        printf("%d\n", nearest_neighbors[i]);
    }

    //predicting the new data
    predict(new_data, k, nearest_neighbors);

    end_s = clock();
    timeSpent_s += (double)(end_s - begin_s) / CLOCKS_PER_SEC;
    printf("\nSequential Time: %f\n", timeSpent_s);

}

void predict(int new_data[2], int k, int nearest_neighbors[k]){

    int fr[k];
    int visited = -1;

    for(int i = 0; i <k; i++){
        int count = 1;
        for(int j = i+1; j < k; j++){
            if(nearest_neighbors[i] == nearest_neighbors[j]){
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

    printf("\nPrediction: %d\n", nearest_neighbors[max_frequency_index]);
    

}

void getNearestNeighbors(int n, int dataset[n][3], double dataset_x[n], int k, int nearest_neighbors[k]){
    //sorting distances
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(dataset_x[i] < dataset_x[j]){
                double temp = dataset_x[i];
                dataset_x[i] = dataset_x[j];
                dataset_x[j] = temp;
                int temp2 = dataset[i][2];
                dataset[i][2] = dataset[j][2];
                dataset[j][2] = temp2;
            }
        }
    }
    //getting the k nearest neighbors
    // printf("The k nearest neighbors are:\n");
    for (int i = 0; i < k; i++){
        nearest_neighbors[i] = dataset[i][2];
        // printf("%d %d %d %f\n", dataset[i][0], dataset[i][1], dataset[i][2], dataset_x[i]);
    }
}


void printDatasetWithEuclideanDistance(int n, int dataset[n][3], double dataset_x[n]){
    for(int i = 0; i < n; i++){
        printf("%d %d %d %f\n", dataset[i][0], dataset[i][1], dataset[i][2], dataset_x[i]);
    }
}

void printDataset(int n, int dataset[n][3]){
    for(int i = 0; i < n; i++){
        printf("%d %d %d\n", dataset[i][0], dataset[i][1], dataset[i][2]);
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
