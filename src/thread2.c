/*
 * Threading with argument passing
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 5

struct thread_data {
	int num;
};

void *PrintHello(void *thread_arg)
{
	struct thread_data *data;
	data = (struct thread_data *)thread_arg;

	printf("%d\n", data->num);
	pthread_exit(NULL);
}

int main()
{
	struct thread_data *arg;
	pthread_t threads[MAX_THREADS];
	int x, rc;

	arg = (struct thread_data *)malloc(sizeof (struct thread_data *));

	for (x = 0; x < MAX_THREADS; x++) {
		arg->num = x;
		rc = pthread_create(&threads[x], NULL, PrintHello, (void *)arg);
		sleep(2);
		if (rc) {
			printf("Error in creating thread\n");
			exit (-1);
		}
	}
	pthread_exit(NULL);
}
