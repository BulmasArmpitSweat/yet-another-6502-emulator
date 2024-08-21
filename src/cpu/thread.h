#ifndef JFKJF_DFII4RNF_XKXJDFJ
#define JFKJF_DFII4RNF_XKXJDFJ

#include "../include/6502-types.h"
#include "../include/stack-manip.h"
#include "../instruction-table.c"
#include "init.h"
#include "mem.h"
#include <bits/pthreadtypes.h>
#include <cstdio>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define QUEUE_LENGTH 10

typedef enum {
    MSG_NOOP,
    MSG_RESET,
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

static inline void queue_init_obj(MessageQueueObj* obj) {
    obj->front = 0;
    obj->rear = 0;
    obj->count = 0;
    pthread_mutex_init(&obj->mutex_lock, NULL);
    pthread_cond_init(&obj->conditional, NULL);
}

static inline void enqueue_message(MessageQueueObj* obj, Message message) {
    pthread_mutex_lock(&obj->mutex_lock);

    // Wait if queue is full
    while (obj->count == QUEUE_LENGTH)
        pthread_cond_wait(&obj->conditional, &obj->mutex_lock);

    obj->queue[obj->rear] = message;
    obj-> rear = (obj->rear + 1) % QUEUE_LENGTH;
    obj->count++;

    pthread_cond_signal(&obj->conditional);
    pthread_mutex_unlock(&obj->mutex_lock);
}

static inline Message dequeue_message(MessageQueueObj* obj) {
    pthread_mutex_lock(&obj->mutex_lock);

    // Wait if queue is empty
    if (obj->count == 0) {
        pthread_mutex_unlock(&obj->mutex_lock);
        return NO_OP;
    }
    while (obj->count == 0)
        pthread_cond_wait(&obj->conditional, &obj->mutex_lock);

    Message message = obj->queue[obj->front];
    obj->front = (obj->front + 1) % QUEUE_LENGTH;
    obj->count--;

    pthread_cond_signal(&obj->conditional);
    pthread_mutex_unlock(&obj->mutex_lock);

    return message;
}

typedef struct {
    MessageQueueObj MObjInit;
    cpu CPUObjInit;
} init_obj;

static inline void* cpu_thread_func(void* arg) {
        init_obj* init = (init_obj*)arg;
        
        cpu* main = &init->CPUObjInit;
        MessageQueueObj* mq = &init->MObjInit;
        queue_init_obj(mq);

        struct timespec ts;

        while (!main->halt) {
            int base_cycles = instructionTable[main->mem[main->PC]].cycles;
            bool page_crossed = !(instructionTable[main->mem[main->PC]].page_crossed_cycle_exception) ? ((main->mem[main->PC] % 256) ? 1 : 0) : 0;
            int extra_cycles = page_crossed && instructionTable[main->mem[main->PC]].page_crossed_cycle_exception ? 1 : 0;

            ts.tv_sec = (base_cycles + extra_cycles) * (main->nanoseconds_per_cycle / NANOSECOND_MULTIPLIER);
            ts.tv_nsec = (base_cycles + extra_cycles) * (main->nanoseconds_per_cycle % NANOSECOND_MULTIPLIER);
            Message message = dequeue_message(mq);
            if (message.type != NO_OP.type) {
                switch (message.type) {
                case MSG_LOAD_CPU_ATTR: {
                    main = (cpu*)message.data;
                    break;
                }
                case MSG_DESTROY: {
                    pthread_mutex_destroy(&mq->mutex_lock);
                    pthread_cond_destroy(&mq->conditional);
                    free(main);
                    free(mq);
                    return NULL;
                }
                case MSG_LOAD_F_STACK: {
                    f_stack_push(main, *(byte_raw*)message.data);
                    break;
                }
                case MSG_LOAD_F_STACK_16: {
                    f_stack_push_16(main, *(byte_raw*)message.data);
                    break;
                }
                case MSG_LOAD_MEMORY: {
                    memory_access_msg_obj mem = *(memory_access_msg_obj*)message.data;
                    PUT_MEM_ADDR(main, mem.addr, mem.data);
                    break;
                }
                case MSG_HALT: {
                    main->halt = true;
                    break;
                }
                case MSG_RESET: {
                    main->PC = RESET_VECTOR;
                    break;
                }
                case MSG_NOOP: {
                    break;
                }
            }
            instructionTable[main->mem[main->PC++]].InstructionPointer(instructionTable[main->mem[main->PC++]].mode, instructionTable[main->mem[main->PC++]].cycles, main, instructionTable[main->mem[main->PC++]].page_crossed_cycle_exception);
            nanosleep(&ts, NULL);
        }
    }
    pthread_mutex_destroy(&mq->mutex_lock);
    pthread_cond_destroy(&mq->conditional);
    free(main);
    free(mq);
    return NULL;
}
#endif /* JFKJF_DFII4RNF_XKXJDFJ */