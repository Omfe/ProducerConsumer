//
//  main.c
//  ProducerConsumer
//
//  Created by Omar Gudino on 9/28/13.
//  Copyright (c) 2013 Omar Gudino. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

sem_t mutex = 1;
sem_t emptyCount = 10;
sem_t fullCount = 0;
int buffer[9];

void producer(void)
{
    while (1) {
        sleep(1);
        int object = rand();
        emptyCount = waitSignal(emptyCount, 1);
        mutex = waitSignal(mutex, 1);
        buffer[fullCount] = object;
        mutex = post(mutex, 1);
        fullCount = post(fullCount, 1);
        printf("The full count is %d\n", fullCount);
    }
}

void consumer(void)
{
    while (1) {
        sleep(1);
        int item;
        fullCount = waitSignal(fullCount, 1);
        mutex = waitSignal(mutex, 1);
        item = buffer[fullCount];
        mutex = post(mutex, 1);
        emptyCount = post(emptyCount, 1);
        item = 0;
        printf("The empty count is %d\n", emptyCount);
    }
}

int main(int argc, const char * argv[])
{
    pthread_t producerT, consumerT;
    
    pthread_create(&producerT, NULL, producer, NULL);
    pthread_create(&consumerT, NULL, consumer, NULL);
    
    pthread_join(consumerT, NULL);
    pthread_join(producerT, NULL);
    return 0;
}


sem_t post(sem_t semaphore, int value)
{
    semaphore = semaphore + value;
    return semaphore;
}

sem_t waitSignal(sem_t semaphore, int value)
{
    while (1) {
        if (semaphore >= 0) {
            semaphore = semaphore - value;
        }
        break;
    }
    return semaphore;
}