#ifndef TASK_I2C_H
#define TASK_I2C_H

bool  taskI2C_enqueue ( struct msg_t msg );
void* taskI2C         ( void*  arg       );

#endif

