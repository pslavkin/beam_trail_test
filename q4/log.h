#ifndef LOG_H
#define LOG_H

#define LOG(fmt, ...) \
   if(LOG_ENABLE==true) { \
    struct timespec ts; \
    clock_gettime(CLOCK_REALTIME, &ts);\
    uint32_t now =  (long long)(ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); \
    printf("\033[38;5;%um" "| %08u | %10s | %25s | "  fmt, LOG_COLOR,now, MODULE,__func__, ##__VA_ARGS__); \
    fflush(stdout); \
   }

#endif // LOG_H
