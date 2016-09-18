#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define N (3)

typedef struct arg_struct_t {
	char* arg1;
	char* arg2;
} arg_struct_t;

void* work(void* argument);
void* work2(void* argument);
void set_thread_type1_attr(pthread_attr_t* attr);
void set_thread_type2_attr(pthread_attr_t* attr);
void cleanup_handler(void* argument); 

void cleanup_handler(void* argument) 
{
	printf("%s%s\n", "cleanup, argument = ", (char*)argument);
	free(argument);
}

void* work(void* argument) 
{
	char* a = (char*)malloc(6*sizeof(char));
	assert(a != NULL);
	
	pthread_cleanup_push(cleanup_handler, a); // first handler
	sprintf(a, "monkey");
	pthread_cancel(pthread_self()); // CANCEL on next cancellation point
	//sleep(1);
	//arg_struct_t* arg = (arg_struct_t*)argument;
	//printf("\n%s%s %s\n\n", "work with: ", arg->arg1, arg->arg2);
	pthread_testcancel(); // cancellation point
	pthread_cleanup_pop(1); // first pop
	
	return (void*)"hey";
}

void* work2(void* argument) 
{
	char* a = (char*)malloc(5*sizeof(char));
	assert(a != NULL);

	pthread_cleanup_push(cleanup_handler, a); // first handler
	sprintf(a, "cycle");
	pthread_cancel(pthread_self()); //CANCEL on next cancellation point
	//arg_struct_t* arg = (arg_struct_t*)argument;
	//printf("\n%s%s %s\n\n", "detached -> work with: ", arg->arg1, arg->arg2);
	//sleep(1);
	pthread_testcancel(); // cancellation point
	pthread_cleanup_pop(1); // first pop

	return (void*)"hey";
}

void set_thread_type1_attr(pthread_attr_t* attr)
{
	int attr_ret;
	attr_ret = pthread_attr_init(attr);
	assert(attr_ret == 0);
	//Use DETACHED if no joining and reuse thread id when terminate.
	//attr_ret = pthread_attr_setdetachstate(attr,PTHREAD_CREATE_DETACHED);
	attr_ret = pthread_attr_setdetachstate(attr,PTHREAD_CREATE_JOINABLE);
	assert(attr_ret == 0);
}

void set_thread_type2_attr(pthread_attr_t* attr)
{
	int attr_ret;
	attr_ret = pthread_attr_init(attr);
	assert(attr_ret == 0);
	//Use DETACHED if no joining and reuse thread id when terminate.
	attr_ret = pthread_attr_setdetachstate(attr,PTHREAD_CREATE_DETACHED);
	//attr_ret = pthread_attr_setdetachstate(attr,PTHREAD_CREATE_JOINABLE);
	assert(attr_ret == 0);
}

int main(int argc, char const *argv[])
{
	int ret;
	pthread_t threads[N];
	pthread_t detached_thread;
	void* values[N];
	pthread_attr_t attr1, attr2;
	set_thread_type1_attr(&attr1);
	set_thread_type2_attr(&attr2);
	arg_struct_t arg_struct={"arg 1", "arg 2"};

	ret = pthread_create(&detached_thread,
				   &attr2,
				   work2,
				   &arg_struct);
	assert(ret == 0);
	printf("%s\n", "detached thread fired");

	for (int i = 0; i < N; ++i)
	{
		ret = pthread_create(&threads[i],
					   &attr1,
					   work,
					   &arg_struct);
		assert(ret == 0);
		printf("%s%d\n", "fired thread ", i);
	}

	for (int i = 0; i < N; ++i)
	{
		ret = pthread_join(threads[i], &values[i]);
		if (ret != 0) {
			handle_error_en(ret, "pthread_join");
		} 
		if (values[i] == PTHREAD_CANCELED) {
        	printf("Thread was canceled; id = %d\n", i);
        } else {
        	printf("Thread terminated normally; id = %d\n", i);
			printf("%s%d%s%s\n", "return value ", i, ": ", (char*)values[i]);
   		}
	}

	ret = pthread_attr_destroy(&attr1);
	assert(ret == 0);
	ret = pthread_attr_destroy(&attr2);
	assert(ret == 0);
	printf("%s\n", "exit program..");
	return 0;
}