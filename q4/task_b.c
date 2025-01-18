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
#include "log.h"

#define MODULE    "TASK_B"
#define LOG_COLOR 3
#define TASK_PERIOD 1 //send i2c very often

static struct queue_t q;

static void i2cEndCallback(bool status) 
{
   struct msg_t msg = {
      .status = status,
   };
   queue_enqueue(&q, msg);
}

void* taskB(void* arg) 
{
    struct msg_t msg;
    struct msg_t msgAck;
    int          i = 0;

    LOG("%s is running.\n", __func__);
    queue_init(&q);
    while(1) {
       msg.address     = i;
       msg.callback    = i2cEndCallback;
       msg.data_length = 1;                 // represent segs, so it's not a big msgs
       msg.data        = (uint8_t*)"taskB"; // fake some data
       taskI2C_enqueue ( msg );
       msgAck = queue_dequeue ( &q );       // queue as a semaphore to wait for the ack
       LOG("%s I2C ack: %u\r\n",__func__,msgAck.status);
       i++;
       sleep(TASK_PERIOD);
    }
    queue_destroy(&q);
    LOG("%s is finished.\n", __func__);
    return NULL;
}
