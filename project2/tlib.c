#include <stdio.h>
#include <stdlib.h>

#include <ucontext.h>
#include "tlib.h"

/* * * * * * * * * * * * * * * * * * *
 * Global variables
 * current  --> Points to the current running element on thread list (TCBLIST)
 * tcb_list --> structure that holds TCB_t
 * * * * * * * * * * * * * * * * * * *
 */
TCB_t *current;
TCB_LIST tcb_list;
#define TCBLIST tcb_list.list

//A solution to getContext and setContext to return twice.
static volatile int isRunning;


int tlib_init (void)
{

    //INITIALIZING CURRENT POINTER AND POINTING IT TO THREAD_0
    current = &(TCBLIST[0]);
    current->ucontext = (ucontext_t *) malloc(sizeof (ucontext_t) );

    //INITIAL CONTEXT SAVING FOR THREAD_0
    if(getcontext(current->ucontext) == CONTEXT_ERROR)
        return (TLIB_ERROR);

    //INITIALIZING THREAD 0 --> MAIN THREAD
    current->t_id = 0;
    current->start_func = NULL;
    current->param = NULL;
    current->isFree = NOT_FREE;

    //INITIALIZING TCBLIST isFree VALUES
    for(int i = 1; i < TLIB_MAX_THREADS; i++){
        TCBLIST[i].isFree = FREE;
    }
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
    /*
     * Initializing new thread
     * ucontext field is allocated in tlib_yield
     * Setting start function to "func"
     * Setting parameters to "param"
     * Flags TCB as NOT_FREE
     *
     * RETURNS --> ID of the created thread or if TLIB_NOMORE if there is no FREE  TCB in TCBLIST
     */

    for(int i = 0; i < TLIB_MAX_THREADS; i++)
        if((TCBLIST[i].isFree) == TRUE) {
            TCBLIST[i].t_id = (unsigned int)i;
            TCBLIST[i].start_func = func;
            TCBLIST[i].param = param;
            TCBLIST[i].isFree = NOT_FREE;
            return (TCBLIST[i].t_id);
        }

    return (TLIB_NOMORE);
}


int tlib_yield(int wantTid)
{
    /*
     * Checks for :
     *  1 -> wantTid is in range of 0 to MAX_THREAD_ID
     *  2 -> TCBLIST[wantTid] is not FREE
     *  If checks fail --> Returns TLIB_INVALID
     *
     * isRunning -> Flag for understanding the current thread
     *
     */

    if(wantTid > MAX_THREAD_ID)
        return (TLIB_INVALID);

    if(TCBLIST[wantTid].isFree == TRUE) {
        return (TLIB_INVALID);
    }

    unsigned int temp = current->t_id;


    if(getcontext(current->ucontext) == CONTEXT_ERROR)
        return (TLIB_ERROR);
    else{
        current = &(TCBLIST[wantTid]);
        if(current->ucontext != NULL){
            if(isRunning == RUNNING){
                isRunning = READY;
                current->prev_id = temp;
                setcontext((current->ucontext));
            }
        } else {
            current->prev_id = temp;
            current->ucontext = (ucontext_t* )malloc( sizeof( ucontext_t) );
            stub(current->start_func, current->param);
            }
    }
}


int tlib_delete_thread(int tid)
{
    if(tid == TLIB_SELF)
        tid = current->t_id;
    printf("Deleting thread id: %d \n" , tid);
    TCBLIST[tid].isFree = FREE;
    tlib_yield(TCBLIST[tid].prev_id); //Give control to previous thread TODO NOT WORKING
    return (TLIB_SUCCESS);
}

