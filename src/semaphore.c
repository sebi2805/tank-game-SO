#include "semaphore.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Funcția pentru inițializarea semafoarelor
sem_t** initialize_semaphores(int rows, int cols) {
    sem_t **sems = (sem_t**)malloc(rows * sizeof(sem_t*));
    if (sems == NULL) {
        perror("Failed to allocate memory for semaphores");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        sems[i] = (sem_t*)malloc(cols * sizeof(sem_t));
        if (sems[i] == NULL) {
            perror("Failed to allocate memory for semaphore row");
            exit(1);
        }

        for (int j = 0; j < cols; j++) {
            if (sem_init(&sems[i][j], 1, 1) == -1) {
                perror("Failed to initialize semaphore");
                exit(1);
            }
        }
    }

    return sems;
}
