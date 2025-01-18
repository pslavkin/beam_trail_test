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

static void i2cEndCallback(bool status) 
{
   struct msg_t msgDummy;

   queue_enqueue(&q, msgDummy);
}

void* taskC(void* arg) 
{
    struct msg_t msg;
    int          i = 0;

    printf("%s is running.\n", __func__);
    queue_init(&q);
    while(1) {
       msg.address     = i;
       msg.callback    = i2cEndCallback;
       msg.data_length = 10;
       msg.data        = (uint8_t*)"taskC msg data 0123456789";
       taskI2C_enqueue ( msg );
       msg = queue_dequeue ( &q ); //queue as a semaphore to wait for the ack
       printf("%s I2C ack\r\n",__func__);
       i++;

       // Simulate some work with sleep
       sleep(1);

    }
    queue_destroy(&q);
    printf("%s is finished.\n", __func__);
    return NULL;
}
