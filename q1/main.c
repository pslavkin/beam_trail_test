#include <unistd.h>
#include <stdbool.h>
#include "adc_measurement_task.h"

#define ADC_TASK_TIME_PERIOD_MS 10 // 100 I'm using 10ms to speed up the example, but it must be 100 as per requirements
int main ( void )
{
   AdcMeasurementTask_Init ( );
   while (true) {
      AdcMeasurementTask ( );
      usleep ( ADC_TASK_TIME_PERIOD_MS*1000 ); //100ms delay. NOTE: no a absolute timer, just a delay for the sake of the example
   }
   return 0;
}
