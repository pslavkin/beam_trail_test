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

pthread_t threadA;
pthread_t threadB;
pthread_t threadC;
pthread_t threadI2C;

int main(void) 
{
   pthread_create(&threadI2C ,NULL ,taskI2C ,NULL);
   pthread_create(&threadA   ,NULL ,taskA   ,NULL);
   pthread_create(&threadB   ,NULL ,taskB   ,NULL);
   pthread_create(&threadC   ,NULL ,taskC   ,NULL);

   pthread_join(threadI2C ,NULL);
   pthread_join(threadC   ,NULL);
   pthread_join(threadB   ,NULL);
   pthread_join(threadA   ,NULL);
   return 0;
}
