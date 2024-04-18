#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX

void floydWarshall(int **dist, int N) {
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

void printSolution(int **dist, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist[i][j] == INF)
                printf("INF ");
            else
                printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("type the number of node and number of undirected edge split with a space: ");
    int N, M;
    scanf("%d %d", &N, &M);

    // Allocate memory for the adjacency matrix
    int **dist = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        dist[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            if (i == j) dist[i][j] = 0; // Distance to itself is 0
            else dist[i][j] = INF; // Initialize distances to INF
        }
    }

    // Read the edges
    printf("type the undirected edge between nodes and the  weight  of  that  edge. Use  positive  edge  weights in the format u x w: \n");
    for (int i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        if (w < 0) {
            printf("Negative edge weight detected. Please use positive weights.\n");
            // Clean up allocated memory
            for (int i = 0; i < N; i++) {
                free(dist[i]);
            }
            free(dist);
            return 1; // Exit with error code
        }
        // Adjust for 0-based indexing and set the distance
        dist[u-1][v-1] = w;
        dist[v-1][u-1] = w; // Since the graph is undirected
    }
    

    //to calculate start time 
    clock_t start = clock();
    // Run Floyd-Warshall algorithm
    floydWarshall(dist, N);
    clock_t end = clock();

    // Print the shortest distance matrix
    printf("The shortest distance matrix is:\n");
    printSolution(dist, N);
    //print time
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    //printf("Computational time: %f seconds\n", time_spent);


    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(dist[i]);
    }
    free(dist);

    return 0;
}
