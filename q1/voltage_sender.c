#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "voltage_sender.h"

void SendVoltage(voltage_t voltage_type, uint32_t value_mv)
{
   printf("     TCP send ch: %u : %u[mV]\r\n" ,voltage_type ,value_mv);
}


