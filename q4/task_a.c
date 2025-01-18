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

#define MODULE    "TASK_A"
#define LOG_COLOR 2
#define TASK_PERIOD 10 //not too frequently as per requisite

static struct queue_t q;

static void i2cEndCallback(bool status) 
{
   struct msg_t msg = {
      .status = status,
   };
   queue_enqueue(&q, msg);
}

void* taskA(void* arg) 
{
    struct msg_t msg;    //to fill the data to send to i2c
    struct msg_t msgAck; //used as s semaphore to wait for the ack
    uint8_t      i = 0;

    LOG("%s is running.\n", __func__);
    queue_init(&q);
    while(1) {
       msg.address     = i;                 // used as a sequence number
       msg.callback    = i2cEndCallback;
       msg.data_length = 10;                // represent segs taskA send big message
       msg.data        = (uint8_t*)"taskA"; // fake some data
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
