#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue_lib.h"
#include "driver_i2c.h"

static pthread_t    thread;
static struct msg_t msg;

void* task(void* arg) 
{
   printf("begin hardware i2c tx that will take: %zu\r\n", msg.data_length);
   sleep(msg.data_length);
   printf("hardware i2c end\r\n");
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
