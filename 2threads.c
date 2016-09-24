#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        {do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0);}
#define status_check(stat) ((stat != 0) ? (handle_error_en(stat, "status error")) : ((void)0))
#define T1(x) printf("%s", "Thread 1: "); x;
#define T2(x) printf("%s", "Thread 2: "); x;        

pthread_mutex_t mutex;
pthread_cond_t cond;

static void * thread_start1(void *arg)
{
	int ret;
	pthread_mutex_lock(&mutex);
	while (1) {
		//sleep(2);
		T1(printf("%s\n", "now waiting for signal.."));
		ret = pthread_cond_wait(&cond,
								&mutex);
		status_check(ret);
		T1(printf("%s\n", "passing through wait condition, (signal recieved)"));
		break;
	}
	pthread_mutex_unlock(&mutex);
    return NULL;
}

static void* thread_start2(void* arg)
{
	int ret;
	ret = pthread_cond_signal(&cond);
	status_check(ret);
	T2(printf("%s\n", "signal sent to thread 1"));
	return NULL;
}

int main(int argc, char const *argv[])
{
	int ret;
	pthread_t thread[2];

	ret = pthread_mutex_init(&mutex, NULL);
	status_check(ret);
	ret = pthread_cond_init(&cond, NULL);
	status_check(ret);

	ret = pthread_create(&thread[0],
						 NULL,
						 thread_start1,
						 NULL);
	status_check(ret);
	ret = pthread_create(&thread[1],
						 NULL,
						 thread_start2,
						 NULL);
	status_check(ret);
	ret = pthread_join(thread[0], NULL);
	status_check(ret);
	ret = pthread_join(thread[1], NULL);
	status_check(ret);
	printf("%s\n", "exit program..");
	return 0;
}