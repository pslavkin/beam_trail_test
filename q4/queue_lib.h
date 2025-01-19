#ifndef QUEUE_LIB_H
#define QUEUE_LIB_H


// Queue structure
struct msg_t {
   union {
      uint8_t address;
      bool    status;
   };
   uint8_t*   data;
   size_t     data_length;
   void       (*callback)(bool);
};

struct queue_t {
    struct          msg_t buffer[ QUEUE_SIZE ];
    int             front;
    int             rear;
    int             count;
    pthread_mutex_t lock;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
};

void         queue_init    ( struct queue_t *q                                      );
bool         queue_enqueue ( struct queue_t *q, struct msg_t value,uint32_t delayMs );
bool         queue_dequeue ( struct queue_t *q, struct msg_t* msg, uint32_t delayMs );
void         queue_destroy ( struct queue_t *q                                      );
#endif
