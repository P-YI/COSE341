#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 5
pthread_t tid[NUM_THREADS];

void *hello_thread(void *arg) {
    int i = 0;
    while(1) {
        if(i==5) {
            break;
        }
        i++;
        sleep(1);
    }
    printf("thread %ld terminated...\n", (intptr_t) arg);
    pthread_exit( (void*)(intptr_t)i);
}

void main() {
    long int i, status;
    printf("pid : %d\n", getpid());

    for(i=0; i<NUM_THREADS; i++) {
        status = pthread_create(&tid[i], NULL, hello_thread, (void *)(intptr_t)i);
        if (status != 0) {
            fprintf(stderr, "create thread error: %s\n", strerror(status));
        }
        printf("thread %ld is created...\n", i);
    }

    for(i=0; i<NUM_THREADS; i++) {
        void *ret;
        status = pthread_join(tid[i], &ret);
        printf("thread %ld is joined...\n", i);
        printf("thread %ld return value: %ld\n", i, (intptr_t)ret);
    }
}