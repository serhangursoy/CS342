#ifndef _ULT_H_
#define _ULT_H_
#include <ucontext.h>

#define TLIB_MAX_THREADS 1024
#define TLIB_MIN_STACK  32768

/* You will define your TCB structure here */
typedef int bool;
#define TRUE 1
#define FALSE 0
#define MAX_THREAD_ID 1023
#define CONTEXT_ERROR -1
#define RUNNING 0
#define READY 1
#define FREE 1
#define NOT_FREE 0


typedef struct TCB_t {
    ucontext_t *ucontext;   //Pointer to CPU context
    unsigned int t_id:10;   //ID--> 0 -- 1023
    void* start_func;
    void* param;
    bool isFree;
    unsigned int state;
    unsigned int prev_id:10;
    //TODO ADD STACK
} TCB_t;

typedef struct TCB_LIST {
    TCB_t list[TLIB_MAX_THREADS];
}TCB_LIST;

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



