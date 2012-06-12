/*
 * Basic threading practice
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 2

void *PrintHello(void *threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Hello from thread %ld!\n", tid);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread[MAX_THREADS];
	int x, rc;

	for (x = 0; x < MAX_THREADS; x++) {
		printf("In main creating thread %d.\n", x);
		rc = pthread_create(&thread[x], NULL, PrintHello, (void *)x);

		if (rc) {
			fprintf(stderr, "ERROR: return code from create is %d.\n", rc);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}
