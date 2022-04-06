#include <stdio.h>
#include <stdlib.h>

void readDatasetFromFile(char * path, int n, int dataset[n][3]);

int main(void){
    int n = 4;
    int dataset[n][3];
    readDatasetFromFile("dataset.txt", n, dataset);
    for(int i = 0; i < n; i++){
        printf("%d %d %d\n", dataset[i][0], dataset[i][1], dataset[i][2]);
    }
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
