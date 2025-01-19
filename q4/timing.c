#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "config.h"
#include "log.h"

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
   return timeout;
}

