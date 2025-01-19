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
#include "task_c.h"
#include "task_i2c.h"

#define MODULE           "TASK_C"
#define LOG_COLOR        4
#define TASK_PERIOD_MS   TASK_C_PERIOD_MS
#define TASK_ENABLE      TASK_C_ENABLE
#define TASK_DATA_LENGTH TASK_C_DATA_LENGTH
#define LOG_ENABLE       TASK_C_LOG_ENABLE

enum taskState_t {
   TASK_STATE_TX,
   TASK_STATE_RX,
};

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
    struct msg_t     msg;
    struct msg_t     msgAck;
    enum taskState_t state = TASK_STATE_TX;
    int              i     = 0;

    queue_init(&q);
    while(true) {
       LOG("loop %u\r\n",i);
       switch(state) {
          case TASK_STATE_TX:
             //fill some fake data to send to i2c
             msg.address     = i;
             msg.callback    = i2cEndCallback;
             msg.data_length = TASK_C_DATA_LENGTH;
             msg.data        = (uint8_t*)MODULE;
             // in case queue is full, drop msg and not wait ack
             if( taskI2C_enqueueNoneBlock ( msg ) ) {
                state = TASK_STATE_RX;
             }
             else {
                LOG("I2C queue is full, drop the msg\r\n");
             }
             break;
          case TASK_STATE_RX:
             //wait for the ack
             if( queue_dequeue ( &q ,&msgAck, 100))  {
                LOG("I2C ack: %u\r\n",msgAck.status);
                state = TASK_STATE_TX;
             }
             break;
          default:
             break;
       }
       i++;
       delayMs(TASK_PERIOD_MS);
    }
    return NULL;
}

//public API
void initTaskC(void)
{
   if(TASK_ENABLE) {
      pthread_create(&thread ,NULL ,task ,NULL);
   }
}
