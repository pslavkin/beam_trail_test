#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "config.h"
#include "log.h"
#include "timing.h"
#include "queue_lib.h"
#include "task_i2c.h"
#include "driver_i2c.h"

#define MODULE     "I2C"
#define LOG_COLOR  7
#define LOG_ENABLE TASK_I2C_LOG_ENABLE

static pthread_t      thread;
static struct queue_t q;
static struct queue_t qAck;

static void i2cEndCallback(bool status) 
{
   struct msg_t msg = {
      .status = status,
   };
   //send the ack to taskI2C
   LOG("status: %u\n", status);
   queue_enqueue(&qAck, msg, MAX_QUEUE_TOUT_MS);
}

static void* task(void* arg) 
{
    struct msg_t msg;
    struct msg_t msgAck;

    LOG("running.\r\n");
    I2C_init();
    queue_init ( &q    ); //queue to store the msg from taskA, taskB, taskC
    queue_init ( &qAck ); //queue to store the ack from i2c driver
    while(true) {
       //wait for the msg from taskA, taskB, taskC
       queue_dequeue(&q, &msg, MAX_QUEUE_TOUT_MS); 
       //send the msg to i2c service and wait for the ack
       I2C_write(msg.address, msg.data, msg.data_length, i2cEndCallback);
       //wait for the ack
       //i2c driver a bit more than expected
       //'cause chip error, sometimes driver fail and don't send the ack
       //in that case I'll ack the client with status = false to take action
       msgAck.status = false;
       queue_dequeue(&qAck, &msgAck, msg.data_length + TASK_I2C_ACK_EXCEED_TIME_MS);
       //call the client callback
       msg.callback(msgAck.status);
    }
    return NULL;
}

//public API
void initTaskI2C(void)
{
   pthread_create(&thread ,NULL ,task ,NULL);
}

bool taskI2C_enqueueNoneBlock(struct msg_t msg) 
{
   //just 10ms as a fake delay
   LOG("%u - %s - %u\r\n",  msg.address, msg.data, 1);
   return queue_enqueue(&q, msg,1);
   //in case of queue is full, drop the msg, so 
//   return queue_enqueueFront(&q, msg,1);
}

// Enqueue an element in case of queue is full, wait until the queue is not full
bool taskI2C_enqueue(struct msg_t msg) 
{
   LOG("%u - %s - %zu\r\n",  msg.address, msg.data, msg.data_length);
   return queue_enqueue(&q, msg, MAX_QUEUE_TOUT_MS);
}
