#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>

#define NUM_THREADS 30

void *hello_thread(void *arg) {
    printf("%ld : Hello world!\n", (long int)arg);
}

main() {
    pthread_t tid[NUM_THREADS];
    int i, status;
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL, hello_thread, (void *)i);
    }
    if (status != 0) {
        printf("Thread creation failed: %d\n", status);
        exit(1);
    }
    pthread_exit(NULL);
}