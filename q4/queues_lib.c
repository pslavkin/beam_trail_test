
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define QUEUE_SIZE 10

// Queue structure
typedef struct {
    int buffer[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Queue;

// Initialize the queue
void init_queue(Queue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

// Enqueue an element
void enqueue(Queue *q, int value) {
    pthread_mutex_lock(&q->lock);

    // Wait if the queue is full
    while (q->count == QUEUE_SIZE) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }

    q->buffer[q->rear] = value;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}

// Dequeue an element
int dequeue(Queue *q) {
    pthread_mutex_lock(&q->lock);

    // Wait if the queue is empty
    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    int value = q->buffer[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    return value;
}

// Clean up the queue
void destroy_queue(Queue *q) {
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

// Task 1: Producer
void *producer(void *arg) {
    Queue *q = (Queue *)arg;
    for (int i = 0; i < 20; i++) {
        printf("Producer: Enqueuing %d\n", i);
        enqueue(q, i);
        sleep(1); // Simulate some work
    }
    return NULL;
}

// Task 2: Consumer
void *consumer(void *arg) {
    Queue *q = (Queue *)arg;
    for (int i = 0; i < 20; i++) {
        int value = dequeue(q);
        printf("Consumer: Dequeued %d\n", value);
        sleep(2); // Simulate some work
    }
    return NULL;
}

int main() {
    Queue q;
    init_queue(&q);

    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, (void *)&q);
    pthread_create(&consumer_thread, NULL, consumer, (void *)&q);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    destroy_queue(&q);
    return 0;
}

