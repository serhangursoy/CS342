#include <assert.h>
#include <stdio.h>
#include "tlib.h"

void *foo(void *v)
{
	int count = 0; 
//	while (count < 100) {
//		printf ("thread %d is running\n", (unsigned int)v);
//		if (count % 10 == 0) {
//			tlib_yield (count);
//		}
//		count++;
//	}

	printf ("thread %d is finished , exiting...\n", (unsigned int)v);
   	 if(((unsigned int) v) < MAX_THREAD_ID - 1000)
        tlib_yield((unsigned int)v + 1);


	return (NULL); 
}


int main(int argc, char **argv)
{
	int tid[1000];
	int c = 0;
	int i; 
	
	tlib_init ();

	for (i = 0; i < 100; ++i) {
		tid[i] = tlib_create_thread ((void *)&foo, (void *)(i+1));
		printf ("thead %d created\n", (int) tid[i]);

	}

	//while (c < 100) {
	  	printf ("thread 1 is running\n");
		//if (c % 10 == 0) {
			tlib_yield (2);
		//}
		//c++;
	//}
	return 0;
}
