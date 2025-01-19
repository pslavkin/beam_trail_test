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
#include "task_b.h"
#include "task_i2c.h"

#define MODULE           "TASK_B"
#define LOG_COLOR        3
#define TASK_PERIOD_MS   TASK_B_PERIOD_MS
#define TASK_ENABLE      TASK_B_ENABLE
#define TASK_DATA_LENGTH TASK_B_DATA_LENGTH
#define LOG_ENABLE       TASK_B_LOG_ENABLE

static pthread_t      thread;
static struct queue_t q;

static void i2cEndCallback(bool status) 
{
   struct msg_t msg = {
      .status = status,
   };
   queue_enqueue(&q, msg, MAX_QUEUE_TOUT_MS);
}

static void* task(void* arg) 
{
    struct msg_t msg;
    struct msg_t msgAck;
    int          i = 0;

    queue_init(&q);
    while(true) {
       LOG("loop %u\r\n",i);
       //fill some fake data to send to i2c
       msg.address     = i;
       msg.callback    = i2cEndCallback;
       msg.data_length = TASK_B_DATA_LENGTH;
       msg.data        = (uint8_t*)MODULE;
       // block forever
       taskI2C_enqueue ( msg );
       //wait for the ack
       queue_dequeue ( &q, &msgAck, MAX_QUEUE_TOUT_MS );
       LOG("I2C ack: %u\r\n",msgAck.status);
       i++;
       delayMs(TASK_PERIOD_MS);
    }
    return NULL;
}

//public API
void initTaskB(void)
{
   if(TASK_ENABLE) {
      pthread_create(&thread ,NULL ,task ,NULL);
   }
}
