#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue_lib.h"
#include "driver_i2c.h"
#include "task_a.h"
#include "task_b.h"
#include "task_c.h"
#include "task_i2c.h"

static struct queue_t q;
static struct queue_t qAck;

bool taskI2C_enqueue(struct msg_t msg) 
{
   return queue_enqueue(&q, msg);
}

static void i2cEndCallback(bool status) 
{
   struct msg_t msgDummy;
   queue_enqueue(&qAck, msgDummy);
}

void* taskI2C(void* arg) 
{
    struct msg_t msg;

    printf("%s is running.\n", __func__);
    queue_init ( &q    );
    queue_init ( &qAck );
    while(true) {
       msg = queue_dequeue(&q); 
       I2C_write(msg.address, msg.data, msg.data_length, i2cEndCallback);
       queue_dequeue(&qAck);
       msg.callback(true);
    }
    queue_destroy(&q);
    printf("%s is finished.\n", __func__);
    return NULL;
}
