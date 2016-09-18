#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void 
cleanup_handler(void *arg)
{
    printf("Called clean-up handler, arg = %d\n",*(int*)arg);
    free(arg);
}

static void *
thread_start(void *arg)
{
   	printf("New thread started\n");
	pthread_cancel(pthread_self()); // CANCEL on next cancellation point
   	int* a = (int*)malloc(sizeof(int));
   	assert(a != NULL);

   	pthread_cleanup_push(cleanup_handler, a); // first handler
   	*a = 1; 
   	int* b = (int*)malloc(sizeof(int));
   	assert(b != NULL);
   	pthread_cleanup_push(cleanup_handler, b); // second handler
   	*b = 2;
   	pthread_testcancel();  // cancellation point 
   	pthread_cleanup_pop(1); // pop second handler
   	pthread_cleanup_pop(1); // pop first handler

    return NULL;
}

int main(int argc, char const *argv[])
{
	int ret;
	void* value;
	pthread_t thread;

	ret = pthread_create(&thread,
						 NULL,
						 thread_start,
						 NULL);
	assert(ret == 0);
	printf("%s\n", "thread fired");
	ret = pthread_join(thread, &value);
	if (ret != 0)
	{
		handle_error_en(ret, "pthread_join");
	}
	if (value == PTHREAD_CANCELED)
	{
		printf("%s\n", "thread cancled");
	} else {
		printf("%s\n", "thread terminated normally");
	}
	printf("%s\n", "exit program..");
	return 0;
}