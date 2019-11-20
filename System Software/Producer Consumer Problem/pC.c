#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

sem_t empty, full, mutex;

int con_count = 0, pro_count = 0;
int buff[BUFFER_SIZE];

void *producer()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);

        buff[pro_count] = i + 1;
        printf("\nProducer produced item %d.\n", buff[pro_count]);

        pro_count++;

        if (pro_count == BUFFER_SIZE)
        {
            pro_count = 0;
        }

        sem_post(&mutex);

        sleep(1);

        sem_post(&full);
    }
}

void *consumer()
{
    int i;

    for (i = 0; i < 10; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        printf("\nConsumer consumed item %d.\n", buff[con_count]);

        con_count++;

        if (con_count == BUFFER_SIZE)
        {
            con_count = 0;
        }

        sem_post(&mutex);

        sleep(1);

        sem_post(&empty);
    }
}

void main()
{
    pthread_t producerThread, consumerThread;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
}