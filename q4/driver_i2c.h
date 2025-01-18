#ifndef DRIVER_I2C_H
#define DRIVER_I2C_H

void I2C_write(uint8_t address, uint8_t* data, size_t data_lenght, void (*callback)(bool));

#endif
   

