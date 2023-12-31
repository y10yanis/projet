#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100
#define BUFFER_SIZE 100
// Yanis Mazouz Groupe 01
struct Element {
    int value;
    int row;
    int col;
};

int B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];
int A[MAX_SIZE][MAX_SIZE];
struct Element T[BUFFER_SIZE];
int n1, m1, n2, m2;

pthread_mutex_t mutex;
pthread_cond_t empty;
pthread_cond_t full;
int T_index = 0;

struct ThreadData {
    int thread_id;
};

void remplir(int mat[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    printf("Entrer les valeurs de la matrice :\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d] : ", i, j);
            scanf("%d", &mat[i][j]);
        }
    }
}

void *producer(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            int result = 0;
            for (int k = 0; k < m1; k++) {  // Utiliser m1 ici au lieu de m2
                result += B[i][k] * C[k][j];
            }

            pthread_mutex_lock(&mutex);
            while (T_index >= BUFFER_SIZE) {
                pthread_cond_wait(&empty, &mutex);
            }
            T[T_index].value = result;
            T[T_index].row = i;
            T[T_index].col = j;
            T_index++;
            printf("Producteur %d : Calcul de A[%d][%d] = %d\n", data->thread_id, i, j, result);
            pthread_cond_signal(&full);
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int consumed = 0;

    while (consumed < n1 * m2) {
        pthread_mutex_lock(&mutex);
        while (T_index == 0) {
            pthread_cond_wait(&full, &mutex);
        }
        T_index--;
        struct Element currentElement = T[T_index];
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        A[currentElement.row][currentElement.col] = currentElement.value;
        printf("Consommateur : A[%d][%d] = %d\n", currentElement.row, currentElement.col, currentElement.value);

        consumed++;
    }

    pthread_exit(NULL);
}

int main() {
    printf("Entrer le nombre de lignes de la matrice B : ");
    scanf("%d", &n1);
    printf("Entrer le nombre de colonnes de la matrice B : ");
    scanf("%d", &m1);
    printf("Entrer le nombre de lignes de la matrice C : ");
    scanf("%d", &n2);
    printf("Entrer le nombre de colonnes de la matrice C : ");
    scanf("%d", &m2);

    if (m1 != n2) {
        printf("Les dimensions ne sont pas compatibles, la multiplication est impossible.\n");
        return 1;
    } else {
        remplir(B, n1, m1);
        remplir(C, m1, m2);  // Utiliser m1 ici au lieu de n2
        printf("Matrice B : \n");

        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                printf("%d\t", B[i][j]);
            }
            printf("\n");
        }

        printf("Matrice C : \n");

        for (int i = 0; i < m1; i++) {  // Utiliser m1 ici au lieu de n2
            for (int j = 0; j < m2; j++) {
                printf("%d\t", C[i][j]);
            }
            printf("\n");
        }
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);

    pthread_t threads[n1];
    pthread_t consumer_thread;

    struct ThreadData thread_data[n1];
    struct ThreadData consumer_data;

    for (int i = 0; i < n1; i++) {
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], NULL, producer, (void *)&thread_data[i]);
    }

    pthread_create(&consumer_thread, NULL, consumer, NULL);

    for (int i = 0; i < n1; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);

    printf("Le produit : \n");
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }

    return 0;
}
