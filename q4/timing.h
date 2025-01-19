#ifndef TIMING_H
#define TIMING_H

void            delayMs    ( uint32_t delayMs );
struct timespec calcFuture ( uint32_t delayMs );

#endif
