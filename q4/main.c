#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "config.h"
#include "timing.h"
#include "queue_lib.h"
#include "task_a.h"
#include "task_b.h"
#include "task_c.h"
#include "task_i2c.h"
#include "log.h"

int main(void) 
{
   //launch task. I2C must be the first to enable the I2C bus
   initTaskI2C ( );
   initTaskA   ( );
   initTaskB   ( );
   initTaskC   ( );

   while(true) {
      sleep(1);
   }

   return 0;
}
