#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue_lib.h"

// Initialize the queue
void queue_init(struct queue_t *q) 
{
    q->front = 0;
    q->rear  = 0;
    q->count = 0;
    pthread_mutex_init ( &q->lock      ,NULL );
    pthread_cond_init  ( &q->not_empty ,NULL );
    pthread_cond_init  ( &q->not_full  ,NULL );
}

// Enqueue an element
bool queue_enqueueFromIsr(struct queue_t *q, struct msg_t value) 
{
   bool ans = false;
   pthread_mutex_lock(&q->lock);

   // Wait if the queue is full
   if (q->count <= QUEUE_SIZE) {

      q->buffer[q->rear] = value;
      q->rear = (q->rear + 1) % QUEUE_SIZE;
      q->count++;
      pthread_cond_signal(&q->not_empty);
      ans = true;
   }
   pthread_mutex_unlock(&q->lock);
   return ans;
}

bool queue_enqueue(struct queue_t *q, struct msg_t value) 
{
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

   return true;
}

// Dequeue an element
struct msg_t queue_dequeue(struct queue_t *q) 
{
    pthread_mutex_lock(&q->lock);

    // Wait if the queue is empty
    while (q->count == 0) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    struct msg_t msg = q->buffer[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    return msg;
}

// Clean up the queue
void queue_destroy(struct queue_t *q) 
{
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}
