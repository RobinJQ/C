#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        {do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0);}
#define status_check(stat) ((stat != 0) ? (handle_error_en(stat, "status error")) : ((void)0))

static void* thread_start(void *arg)
{
	printf("%s\n", "thread working..");
	sleep(1);
    return NULL;
}

int main(int argc, char const *argv[])
{
	int status;
	pthread_t thread;

	status = pthread_create(&thread,
						    NULL,
						    thread_start,
						    NULL);
	status_check(status);
	status = pthread_join(thread, NULL);
	status_check(status);
	printf("%s\n", "exit program..");
	return 0;
}