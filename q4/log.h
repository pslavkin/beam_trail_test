#ifndef LOG_H
#define LOG_H

#define LOG(fmt, ...) printf("\033[38;5;%um" "| %10s | %25s | " fmt, LOG_COLOR,MODULE,__func__, ##__VA_ARGS__)

#endif // LOG_H
