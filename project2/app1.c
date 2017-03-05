#include <assert.h>
#include <stdio.h>
#include "tlib.h"

void *foo(void *v)
{
	int count = 0; 
	while (count < 1000) {
		printf ("thread %d is running\n", (unsigned int)v);
		if (count % 100 == 0) {
			// tlib_yield (....);
		}
		count++; 
	}
	return (NULL); 
}


int 
main(int argc, char **argv)
{
	int tid[10];
	int c = 0;
	int i; 
	
	tlib_init ();

	for (i = 0; i < 10; ++i) {
		tid[i] = tlib_create_thread ((void *)&foo, (void *)(i+2));
		printf ("thead %d created\n", (int) tid[i]);

	}
		
	while (c < 1000) {
	  	printf ("thread 1 is running\n"); 
		if (c % 100 == 0) {
			// tlib_yield (.....);
		}
		c++;
	}
	return 0;
}
