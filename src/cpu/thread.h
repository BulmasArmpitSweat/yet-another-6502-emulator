#ifndef JFKJF_DFII4RNF_XKXJDFJ
#define JFKJF_DFII4RNF_XKXJDFJ

#include "../include/6502-types.h"
#include "../include/stack-manip.h"
#include "../instruction-table.c"
#include "mem.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define QUEUE_LENGTH 10

typedef enum {
    MSG_NOOP,
    MSG_RESET,
    MSG_PAUSE,
    MSG_RESUME,
    MSG_STEP,
    MSG_HALT,
    MSG_LOAD_F_STACK,
    MSG_LOAD_F_STACK_16,
    MSG_LOAD_MEMORY,
    MSG_LOAD_CPU_ATTR,
    MSG_DESTROY
} Msg_Type;

typedef struct  __attribute__((packed)) {
    ushort addr;
    byte_raw data;
} memory_access_msg_obj;

typedef struct {
    Msg_Type type;
    void* data;
} Message;

typedef struct {
    Message queue[QUEUE_LENGTH];
    int front, rear, count;
    pthread_mutex_t mutex_lock;
    pthread_cond_t conditional;
} MessageQueueObj;

static Message NO_OP = { .type = MSG_NOOP };
static MessageQueueObj mq;

typedef struct {
    MessageQueueObj MObjInit;
    cpu CPUObjInit;
} init_obj;

void queue_init_obj(MessageQueueObj* obj);
void enqueue_message(Message message);
Message dequeue_message();
void* cpu_thread_func(void* arg);

#endif /* JFKJF_DFII4RNF_XKXJDFJ */