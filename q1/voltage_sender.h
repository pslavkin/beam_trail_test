#ifndef VOLTAGE_SENDER_H
#define VOLTAGE_SENDER_H


typedef enum _voltage_t {
   VOLTAGE_A,
   VOLTAGE_B,
   VOLTAGE_C,
   VOLTAGE_D,
} voltage_t;

void SendVoltage(voltage_t voltage_type, uint32_t value_mv);

#endif
   

