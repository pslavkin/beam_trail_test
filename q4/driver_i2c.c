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
#include "driver_i2c.h"

#define MODULE     "DRIVER_I2C"
#define LOG_COLOR  6
#define LOG_ENABLE I2C_DRIVER_LOG_ENABLE

static pthread_t      thread;
static struct queue_t q;

//this task emulate the hardware + isr handler i2c driver that send the i2c data
static void* task(void* arg) 
{
   struct msg_t msg;

   LOG("running.\r\n");
   queue_init ( &q    ); //queue to store the msg from taskA, taskB, taskC
   while(true) {
      //get the msg from the task
      queue_dequeue(&q, &msg, MAX_QUEUE_TOUT_MS); 
      //data_lenght as a fake delay
      LOG("tx begin: %u - %s - %zu[ms]\r\n",msg.address, msg.data, msg.data_length);
      //this is the fake time the i2c driver take to send the data
      delayMs(msg.data_length);
      LOG("tx end  : %u - %s - %zu[ms]\r\n",msg.address, msg.data, msg.data_length);
      //call i2c 'service' callback, not the client, so the service has the
      //opportunity to send the ack and manage any error
      //generate errata error randomply once every 10 tx
      if( I2C_DRIVER_ERRATA_RATE==0 || rand() % I2C_DRIVER_ERRATA_RATE != 0 ) {
         msg.callback(true);
      }
      else {
         LOG("I2C driver errata error => don't send cback\r\n");
      }
   }
   return NULL;
}

//public API
void I2C_write(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool))
{
   struct msg_t msg = {
      .address     = address,
      .data        = data,
      .data_length = data_length,
      .callback    = callback,
   };
   queue_enqueue(&q, msg, MAX_QUEUE_TOUT_MS);
}

void I2C_init(void)
{
   pthread_create(&thread ,NULL ,task ,NULL);
}
