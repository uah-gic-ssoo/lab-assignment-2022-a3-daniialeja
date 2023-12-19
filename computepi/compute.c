#include "compute.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Number of hits */
unsigned int hits = 0;

/* Mutex Semaphore */
pthread_mutex_t mutex;

/* Thread function */
void *thread_function(void *data) {
    /* The input variable contains the number of points that the thread must
     * calculate */
    int points = *((int *)data);

    /* The local variable that shall be used to store the number of points within
     * the circular section */
    int count = 0;

    /* TODO: Implement the loop that obtains the random points and counts how
     * many of those lay within the circumference of radius 1 */
    double random;
    struct drand48_data rand_buffer;
    srand48_r(time(NULL), &rand_buffer);

    for (int i = 0; i < points; i++) {
        drand48_r(&rand_buffer, &random);
        double x = 2 * random - 1;
        drand48_r(&rand_buffer, &random);
        double y = 2 * random - 1;
        double distance = x * x + y * y;

        if (distance <= 1.0) {
            count++;
        }
    }

    /* TODO: Add the count to the global variable hits in mutual exclusion */
    pthread_mutex_lock(&mutex);
    hits += count;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void compute(int npoints, int nthreads) {
    pthread_t hilos[nthreads];
    int puntos_por_hilo = npoints / nthreads;

    /* TODO: Initialize the mutex */
    pthread_mutex_init(&mutex, NULL);

    /* TODO: Launch the threads that will count the points */
    for (int i = 0; i < nthreads; i++) {
        pthread_create(&hilos[i], NULL, thread_function, &puntos_por_hilo);
    }

    /* TODO: Wait for all threads to finish */
    for (int i = 0; i < nthreads; i++) {
        pthread_join(hilos[i], NULL);
    }

    /* TODO: Destroy the mutex */
    pthread_mutex_destroy(&mutex);

    double ratio = (double)(hits * 4) / npoints;

    /* TODO: print the ratio of points that meet the criteria */
    printf("%.8f\n", ratio);

    /* The following print string can be used to print the calculated value:
     * printf("%.8f\n", VALUE_OF_PI);
     * where VALUE_OF_PI is the floating-point value to be printed.
     */
}

