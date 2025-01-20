#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "config.h"
#include "log.h"

#define MODULE           "TIMING"
#define LOG_COLOR        13
#define LOG_ENABLE       TIMING_LOG_ENABLE

void delayMs(uint32_t delayMs) 
{
    struct timespec ts;

    ts.tv_sec = delayMs / 1000;              // Seconds
    ts.tv_nsec = (delayMs % 1000) * 1000000; // Nanoseconds
    nanosleep(&ts, NULL);
}

struct timespec calcFuture(uint32_t delayMs)
{
   struct timeval now;
   struct timespec timeout;

   gettimeofday(&now, NULL);
   timeout.tv_sec = now.tv_sec + delayMs/1000;
   timeout.tv_nsec = now.tv_usec * 1000 + (delayMs%1000)*1000000;
   // if nanoseconds exceed 1 second, increment seconds and decrement nanoseconds
   if(timeout.tv_nsec >= 1000000000) {
      timeout.tv_sec++;
      timeout.tv_nsec -= 1000000000;
   }
   LOG("timeout: %lu %lu\r\n",timeout.tv_sec,timeout.tv_nsec);
   return timeout;
}

