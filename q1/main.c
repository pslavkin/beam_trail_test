#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc_measurement_task.h"
#include <time.h>

int main(void)
{
   AdcMeasurementTask_Init ( );
   while (true) {
      AdcMeasurementTask (          );
      usleep             ( 1*1000 ); //100ms delay. NOTE: no a absolute timer, just a delay for the sake of the example
   }
   return 0;
}
