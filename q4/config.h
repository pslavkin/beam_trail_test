#ifndef CONFIG_H
#define CONFIG_H

//TaskA sends big qty of data, but not too often
#define TASK_A_ENABLE         true
#define TASK_A_PERIOD_MS      1000  //10s
#define TASK_A_DATA_LENGTH    1000 //fake tx time
#define TASK_A_LOG_ENABLE     true

//TaskB send small qty of data, but very often
#define TASK_B_ENABLE         true
#define TASK_B_PERIOD_MS      50 //1s
#define TASK_B_DATA_LENGTH    10  //fake tx time
#define TASK_B_LOG_ENABLE     true

//TaskC send small qty of data, but at high priority and can't block (i.e. ISR)
#define TASK_C_ENABLE         true
#define TASK_C_PERIOD_MS      100
#define TASK_C_DATA_LENGTH    10  //fake tx time
#define TASK_C_LOG_ENABLE     true

#define LOG_QUEUE_ENABLE      true
#define TASK_I2C_LOG_ENABLE   false
#define I2C_DRIVER_LOG_ENABLE true
#define TIMING_LOG_ENABLE     false

#define MAX_QUEUE_TOUT_MS     1000000 //simulate 'forever' (1000s)
#define QUEUE_SIZE            10
#define QUEUE_MARGIN          2

#define I2C_DRIVER_ERRATA_RATE      10
#define TASK_I2C_ACK_EXCEED_TIME_MS 2000
#endif
