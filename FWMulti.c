#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX

int **dist;
int N, M;
pthread_mutex_t mutex;

typedef struct {
    int k;
    int i;
} thread_arg;

void* runFloydWarshall(void* arg) {
    thread_arg* t_arg = (thread_arg*)arg;
    int k = t_arg->k, i = t_arg->i;

    for (int j = 0; j < N; j++) {
        pthread_mutex_lock(&mutex); // Ensure that dist[i][k] + dist[k][j] read is atomic
        if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
            dist[i][j] = dist[i][k] + dist[k][j];
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void floydWarshall() {
    pthread_t threads[N];
    thread_arg t_args[N];

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            t_args[i].k = k;
            t_args[i].i = i;
            pthread_create(&threads[i], NULL, runFloydWarshall, (void*)&t_args[i]);
        }
        for (int i = 0; i < N; i++) {
            pthread_join(threads[i], NULL);
        }
    }
}

void printSolution() {
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
    printf("Type the number of nodes and number of undirected edges split with a space: ");
    scanf("%d %d", &N, &M);

    dist = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        dist[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = INF;
        }
    }
    //read the edges
    printf("Type the undirected edge between nodes and the weight of that edge in the format u v w: \n");
    for (int i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        if (w < 0) {
            printf("Negative edge weight detected. Please use positive weights.\n");
            return 1;
        }
        dist[u-1][v-1] = w;
        dist[v-1][u-1] = w;
    }

    pthread_mutex_init(&mutex, NULL);
    
    //to calculate start time 
    clock_t start = clock();
    // Run Floyd-Warshall algorithm
    floydWarshall();
    clock_t end = clock();

    // Print the shortest distance matrix
    printf("The shortest distance matrix is:\n");
    printSolution();
    //print time
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    //printf("Computational time: %f seconds\n", time_spent);
    
    // Free allocated memory
    for (int i = 0; i < N; i++) {
        free(dist[i]);
    }
    free(dist);

    pthread_mutex_destroy(&mutex);

    return 0;
}
