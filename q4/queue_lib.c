#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "config.h"
#include "log.h"
#include "timing.h"
#include "queue_lib.h"

#define MODULE    "QUEUE"
#define LOG_COLOR  10
#define LOG_ENABLE LOG_QUEUE_ENABLE

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

void queue_empty(struct queue_t *q) 
{
   struct msg_t msg;
   while(queue_dequeue(q, &msg, 0)) {
      LOG("empty %u - %s\r\n",msg.address, msg.data);
      ;
   }
}

bool queue_enqueue(struct queue_t *q, struct msg_t msg,uint32_t toutMs) 
{
   int             ret;
   struct timespec timeout;
   bool            ans = false;

   pthread_mutex_lock(&q->lock);
   timeout = calcFuture(toutMs);
   ret = 0;
   while (q->count == (QUEUE_SIZE - QUEUE_MARGIN) && ret == 0) {
      ret = pthread_cond_timedwait(&q->not_full, &q->lock, &timeout);
   }
   if (ret != 0) { //ETIMEDOUT
      LOG("--> timeout, drop %u - %s\r\n",msg.address, msg.data);
   } else {
      q->buffer[q->rear] = msg;
      q->rear = (q->rear + 1) % QUEUE_SIZE;
      q->count++;
      pthread_cond_signal(&q->not_empty);
      ans = true;
   }
   pthread_mutex_unlock(&q->lock);
   return ans;
}

bool queue_enqueueFront(struct queue_t *q, struct msg_t msg,uint32_t toutMs) 
{
   int             ret;
   struct timespec timeout;
   bool            ans = false;

   pthread_mutex_lock(&q->lock);
   timeout = calcFuture(toutMs);
   ret = 0;
   while (q->count == QUEUE_SIZE && ret == 0) {
      ret = pthread_cond_timedwait(&q->not_full, &q->lock, &timeout);
   }
   if (ret != 0) { //ETIMEDOUT
      LOG("--> timeout, drop %u - %s\r\n",msg.address, msg.data);
   } else {
      q->front = (q->front - 1) % QUEUE_SIZE;
      q->buffer[q->front] = msg;
      q->count++;
      pthread_cond_signal(&q->not_empty);
      ans = true;
   }
   pthread_mutex_unlock(&q->lock);
   return ans;
}

// Dequeue an element
bool queue_dequeue(struct queue_t *q, struct msg_t* msg, uint32_t toutMs) 
{
   int             ret;
   struct timespec timeout;
   bool            ans = false;

   pthread_mutex_lock(&q->lock);
   timeout = calcFuture(toutMs);
   ret = 0;
   while (q->count == 0 && ret == 0) {
      ret = pthread_cond_timedwait(&q->not_empty, &q->lock, &timeout);
   }
   if (ret !=0 ) { //ETIMEDOUT
      LOG("--> timeout\r\n");
   } else {
      *msg = q->buffer[q->front];
      q->front = (q->front + 1) % QUEUE_SIZE;
      q->count--;
      ans = true;
      pthread_cond_signal(&q->not_full);
   }
   pthread_mutex_unlock(&q->lock);
   return ans;
}

// Clean up the queue
void queue_destroy(struct queue_t *q) 
{
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}
