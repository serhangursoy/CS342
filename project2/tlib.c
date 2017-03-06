#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <assert.h>

#include <ucontext.h>
#include "tlib.h"

bool ids[MAX_THREAD_ID + 1];
TCB_t *current;
int tlib_init (void)
{

    current = (TCB_t*) malloc(sizeof(struct TCB_t));
    current->ucontext = (ucontext_t *) malloc(sizeof(ucontext_t));
    if(getcontext(current->ucontext) != 0)
        return (TLIB_ERROR);
    current->priority = HIGH_PRIORITY;
    current->t_id = 0;

    ids[0] = TRUE;
    for(int i = 1; i <= MAX_THREAD_ID; i++)
        ids[i] = FALSE;

    return (TLIB_ERROR);
}

/* implementation of stub is already given below */
void stub (void (*tstartf)(void *), void *arg)
{

	tstartf (arg); /* calling thread start function to execute */
    /* 
        We are done with executing the application specified thread start
        function. Hence we can now terminate the thread
    */
	tlib_delete_thread(TLIB_SELF);
    exit(0);
}


int tlib_create_thread(void (*func)(void *), void *param)
{
    TCB_t *tcb = (TCB_t *) malloc(sizeof(struct TCB_t));
    tcb->ucontext = (ucontext_t *) malloc(sizeof(ucontext_t));
    for(TCB_t *t = current; t != NULL; t = t->next_T)
        if(t->next_T == NULL) {
            t->next_T = tcb;
            break;
        }

    for(unsigned int i = 0; i < MAX_THREAD_ID; i++)
        if(ids[i] == FALSE){
            ids[i] = TRUE;
            tcb->t_id = i;
            break;
        }

    tcb->start_func = func;
    tcb->priority = LOW_PRIORITY;
    tcb->param = param;

    //stub(*func , param);




    return (tcb->t_id);
}


int tlib_yield(int wantTid)
{
	return (TLIB_ERROR);
}


int tlib_delete_thread(int tid)
{
    return (TLIB_ERROR);
}

