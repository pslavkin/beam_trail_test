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

static pthread_t    thread;
static struct msg_t msg;


//this task emulate the hardware + isr handler i2c driver that send the i2c data
void* task(void* arg) 
{
   //data_lenght as a fake delay
   LOG("tx begin: %u - %s - %zu[ms]\r\n",msg.address, msg.data, msg.data_length);
   //this is the fake time the i2c driver take to send the data
   delayMs(msg.data_length);
   LOG("tx end  : %u - %s - %zu[ms]\r\n",msg.address, msg.data, msg.data_length);
   //call i2c 'service' callback, not the client, so the service has the
   //opportunity to send the ack and manage any error
   msg.callback(true);
   return NULL;
}

void I2C_write(uint8_t address, uint8_t* data, size_t data_length, void (*callback)(bool))
{
   msg.address     = address;
   msg.data        = data;
   msg.data_length = data_length;
   msg.callback    = callback;

   pthread_create(&thread ,NULL ,task ,&msg);
}
