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

static inline void queue_init_obj(MessageQueueObj* obj) {
    mq.front = 0;
    mq.rear = 0;
    mq.count = 0;
    pthread_mutex_init(&mq.mutex_lock, NULL);
    pthread_cond_init(&mq.conditional, NULL);
}

static inline void enqueue_message(Message message) {
    pthread_mutex_lock(&mq.mutex_lock);

    // Wait if queue is full
    while (mq.count == QUEUE_LENGTH)
        pthread_cond_wait(&mq.conditional, &mq.mutex_lock);

    mq.queue[mq.rear] = message;
    mq. rear = (mq.rear + 1) % QUEUE_LENGTH;
    mq.count++;

    pthread_cond_signal(&mq.conditional);
    pthread_mutex_unlock(&mq.mutex_lock);
}

static inline Message dequeue_message() {
    pthread_mutex_lock(&mq.mutex_lock);

    // Wait if queue is empty
    if (mq.count == 0) {
        pthread_mutex_unlock(&mq.mutex_lock);
        return NO_OP;
    }
    while (mq.count == 0)
        pthread_cond_wait(&mq.conditional, &mq.mutex_lock);

    Message message = mq.queue[mq.front];
    mq.front = (mq.front + 1) % QUEUE_LENGTH;
    mq.count--;

    pthread_cond_signal(&mq.conditional);
    pthread_mutex_unlock(&mq.mutex_lock);

    return message;
}

typedef struct {
    MessageQueueObj MObjInit;
    cpu CPUObjInit;
} init_obj;

static inline void* cpu_thread_func(void* arg) {
        bool paused = true;
        int step = 0;
        init_obj* init = (init_obj*)arg;
        
        cpu* main = &init->CPUObjInit;
        mq = init->MObjInit;
        queue_init_obj(&mq);

        struct timespec ts;

        while (!main->halt) {
            int base_cycles = instructionTable[main->mem[main->PC]].cycles;
            bool page_crossed = !(instructionTable[main->mem[main->PC]].page_crossed_cycle_exception) ? ((main->mem[main->PC] % 256) ? 1 : 0) : 0;
            int extra_cycles = page_crossed && instructionTable[main->mem[main->PC]].page_crossed_cycle_exception ? 1 : 0;

            ts.tv_sec = (base_cycles + extra_cycles) * (main->nanoseconds_per_cycle / NANOSECOND_MULTIPLIER);
            ts.tv_nsec = (base_cycles + extra_cycles) * (main->nanoseconds_per_cycle % NANOSECOND_MULTIPLIER);
            Message message = dequeue_message();
            if (message.type != NO_OP.type) {
                switch (message.type) {
                case MSG_LOAD_CPU_ATTR: {
                    main = (cpu*)message.data;
                    break;
                }
                case MSG_DESTROY: {
                    pthread_mutex_destroy(&mq.mutex_lock);
                    pthread_cond_destroy(&mq.conditional);
                    free(main);
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
                case MSG_PAUSE: {
                    paused = true;
                    break;
                }
                case MSG_RESUME: {
                    paused = false;
                }
                case MSG_STEP: {
                    step += *(int*)message.data;
                }
                case MSG_RESET: {
                    main->PC = RESET_VECTOR;
                    break;
                }
                case MSG_NOOP: {
                    break;
                }
            }
            if (paused == true && step == 0) {
                continue;
            }
            instructionTable[main->mem[main->PC++]].InstructionPointer(instructionTable[main->mem[main->PC++]].mode, instructionTable[main->mem[main->PC++]].cycles, main, instructionTable[main->mem[main->PC++]].page_crossed_cycle_exception);
            nanosleep(&ts, NULL);
            step--;
        }
    }
    pthread_mutex_destroy(&mq.mutex_lock);
    pthread_cond_destroy(&mq.conditional);
    free(main->configured_hertz);
    free(main);
    return NULL;
}
#endif /* JFKJF_DFII4RNF_XKXJDFJ */