#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <assert.h>

#include <ucontext.h>
#include "tlib.h"


int tlib_init (void)
{
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
    return (TLIB_ERROR);
}


int tlib_yield(int wantTid)
{
	return (TLIB_ERROR);
}


int tlib_delete_thread(int tid)
{
    return (TLIB_ERROR);
}

