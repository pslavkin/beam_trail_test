#ifndef TASK_I2C_H
#define TASK_I2C_H

void initTaskI2C              ( void             );
bool taskI2C_enqueueNoneBlock ( struct msg_t msg );
bool taskI2C_enqueue          ( struct msg_t msg );

#endif

