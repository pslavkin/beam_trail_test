#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue_lib.h"
#include "task_a.h"
#include "task_b.h"
#include "task_c.h"
#include "task_i2c.h"

static struct queue_t q;

bool taskB_enqueue(struct msg_t msg) 
{
   queue_enqueue(&q, msg);
   return 1;
}

static void i2cEndCallback(bool status) 
{
   printf("Task %s callback: %d\n", __func__,status);
}

void* taskB(void* arg) 
{
    struct msg_t msg;
    int          i = 0;

    printf("%s is running.\n", __func__);
    queue_init(&q);
    while(1) {
       msg.address=i;
       msg.callback = i2cEndCallback;
       taskI2C_enqueue(msg);
       msg = queue_dequeue(&q); 
       printf("Message: %u\n", msg.address);
       i++;

       // Simulate some work with sleep
       sleep(1);

    }
    queue_destroy(&q);
    printf("%s is finished.\n", __func__);
    return NULL;
}
