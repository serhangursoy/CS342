#ifndef _ULT_H_
#define _ULT_H_
#include <ucontext.h>

#define TLIB_MAX_THREADS 1024
#define TLIB_MIN_STACK  32768

/* You will define your TCB structure here */
typedef int bool;
#define TRUE 1
#define FALSE 0
#define MAX_THREAD_ID 255
#define HIGH_PRIORITY 0
#define LOW_PRIORITY 1


typedef struct TCB_t {
    ucontext_t *ucontext;   //Pointer to CPU context
    struct TCB_t *next_T;   //Pointer to next thread's TCB
    struct TCB_t *prev_T;   //Pointer to previous threas's TCB
    unsigned int priority:1;//Priority --> 0 -- 1
    unsigned int t_id:8;    //ID       --> 0 -- 256
    void* start_func;
    void* param;

    //TODO ADD STACK
} TCB_t;

/* Some definitions below */

static const int TLIB_ANY = -1;
static const int TLIB_SELF = -2;
static const int TLIB_INVALID = -3;
static const int TLIB_NONE = -4;
static const int TLIB_NOMORE = -5;
static const int TLIB_NOMEMORY = -6;
static const int TLIB_FAILED = -7;
static const int TLIB_SUCCESS = -8;
static const int TLIB_ERROR = -9;


int tlib_init(void);
int tlib_create_thread (void (*func)(void *), void *param);
int tlib_yield (int tid);
int tlib_delete_thread (int tid);

#endif



