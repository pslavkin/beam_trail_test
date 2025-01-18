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

#define MODULE    "TASK_A"
#define LOG_COLOR 2
#define LOG(fmt, ...) printf("\033[38;5;%um" "| %10s | %20s | " fmt, LOG_COLOR,MODULE,__func__, ##__VA_ARGS__)

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
    struct msg_t msg;
    struct msg_t msgAck;
    int          i = 0;

    LOG("%s is running.\n", __func__);
    queue_init(&q);
    while(1) {
       msg.address     = i;
       msg.callback    = i2cEndCallback;
       msg.data_length = 30;
       msg.data        = (uint8_t*)"taskA msg data 0123456789";
       taskI2C_enqueue ( msg );
       msgAck = queue_dequeue ( &q ); //queue as a semaphore to wait for the ack
       LOG("%s I2C ack: %u\r\n",__func__,msgAck.status);
       i++;

       // Simulate some work with sleep
       sleep(1);

    }
    queue_destroy(&q);
    LOG("%s is finished.\n", __func__);
    return NULL;
}
