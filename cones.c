#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#ifdef DEBUG
#  define D(x) x
#else
#  define D(x) 
#endif

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define NR_COSTUMERS (40)

typedef struct inspector_t {
	int pass;
	int finished;
	int request;
	pthread_cond_t finished_cond;
	pthread_cond_t request_cond;
	pthread_mutex_t lock;
	pthread_mutex_t inspection;
} inspector_t;

typedef struct line_t {
	int currentLineNumber;
	int costumers[NR_COSTUMERS];
	pthread_cond_t costumerInLine;
	pthread_cond_t costumers_cond;
	pthread_mutex_t lock;
	pthread_mutex_t costumers_mutex[NR_COSTUMERS];
	pthread_mutex_t cashier_mutex;
	pthread_mutex_t checkout_mutex[NR_COSTUMERS];
} line_t;

typedef struct costumer_arg_t {
	int id;
	int orderCones;
} costumer_arg_t;

typedef struct manager_arg_t {
	int totalCones;
} manager_arg_t;

void* Costumer(void* arg);
void* Clerk(void* arg);
void* Manager(void* arg);
void* Cashier(void* arg);
int inspectCone();
void makeCone();
int getNextLineNumber();
int getCurrentLineNumber();
void Checkout(int i);
void initInspector();
void initLine();
int lineNumberCheckedOut(int lineNumber);

//Global
inspector_t inspector;
line_t line;


int main(int argc, char const *argv[])
{
	pthread_t costumers[NR_COSTUMERS];
	pthread_t manager;
	pthread_t cashier;
	pthread_attr_t manager_attr, cashier_attr;
	costumer_arg_t costumer_args[NR_COSTUMERS];
	manager_arg_t manager_args;
	int status;
	int i;
	int totalCones = 0;

	initInspector();
	initLine();

	for (i = 0; i < NR_COSTUMERS; ++i)
	{
		costumer_args[i].id = i;
		costumer_args[i].orderCones = ( rand() % 4 ) + 1;
		//new Costumer thread;
		status = pthread_create(&costumers[i],
								NULL,
								Costumer,
								&costumer_args[i]);
		assert(status == 0);
		totalCones += costumer_args[i].orderCones;
	}
	//new Manager thread
	manager_args.totalCones = totalCones;
	pthread_attr_init(&manager_attr);
	pthread_attr_setdetachstate(&manager_attr,
								PTHREAD_CREATE_DETACHED);
	status = pthread_create(&manager,
							&manager_attr,
							Manager,
							&manager_args);
	if(status != 0) {
		handle_error_en(status, "pthread_create");
	}
	//new Cashier thread
	pthread_attr_init(&cashier_attr);
	pthread_attr_setdetachstate(&cashier_attr,
								PTHREAD_CREATE_DETACHED);
	status = pthread_create(&cashier,
							&cashier_attr,
							Cashier,
							NULL);
	if (status != 0) {
		handle_error_en(status, "pthread_create");
	}
	//join
	for (int i = 0; i < NR_COSTUMERS; ++i)
	{
		status = pthread_join(costumers[i], NULL);
		assert(status == 0);
	}
	D(printf("totalCones = %d\n", totalCones));
	//free up space
	pthread_attr_destroy(&manager_attr);
	pthread_attr_destroy(&cashier_attr);
	return 0;
}

void* Manager(void* arg) {
	manager_arg_t* arg_t = (manager_arg_t*)arg;
	int passedCones = 0;
	int numInspections = 0;
	D(printf("%s\n", "manager started"));
	while (passedCones < arg_t->totalCones) {
		D(printf("%s\n", "manager waits for inspection"));
		pthread_mutex_lock(&inspector.inspection);
		D(printf("%s\n", "manager waits for request"));
		while(!inspector.request) {
			pthread_cond_wait(&inspector.request_cond, 
							  &inspector.inspection);
		}
		inspector.request = 0;
		inspector.pass = inspectCone();
		++numInspections;
		if (inspector.pass) {
			++passedCones;
		}
		D(printf("%s\n", "manager signals finished"));
		inspector.finished = 1;
		pthread_cond_signal(&inspector.finished_cond);
		pthread_mutex_unlock(&inspector.inspection);
	}
	D(printf("Inspection success rate %d%%\n", (100*passedCones)/numInspections));
	return NULL;
}

void* Clerk(void* arg) {
	int pass = 0;
	while (!pass) {
		makeCone();
		D(printf("%s\n", "clerk waits for lock"));
		pthread_mutex_lock(&inspector.lock);
		D(printf("%s\n", "clerk waits for inspection"));
		pthread_mutex_lock(&inspector.inspection);
		D(printf("%s\n", "clerk signals request"));
		inspector.request = 1;
		pthread_cond_signal(&inspector.request_cond);
		D(printf("%s\n", "clerk waits for finished"));
		while(!inspector.finished) {
			pthread_cond_wait(&inspector.finished_cond, 
							  &inspector.inspection);
		}
		inspector.finished = 0;
		pass = inspector.pass;
		D(printf("%s\n", "clerk unlocks inspection"));
		pthread_mutex_unlock(&inspector.inspection);
		pthread_mutex_unlock(&inspector.lock);
	}
	return NULL;
} 

void* Costumer(void* arg) {
	int i, status;
	costumer_arg_t* arg_t = (costumer_arg_t*)arg;
	pthread_t clerks[arg_t -> orderCones];

	for (i = 0; i < arg_t->orderCones; ++i)
	{
		status = pthread_create(&clerks[i],
								NULL,
								Clerk,
								NULL);
		assert(status == 0);
		D(printf("Customer %d place order\n", arg_t->id));
	}

	for (int i = 0; i < arg_t->orderCones; ++i)
	{
		status = pthread_join(clerks[i], NULL);
		assert(status == 0);
	}
	D(printf("Costumer %d takes line number..\n", arg_t->id));
	int lineNumber = getNextLineNumber();
	pthread_cond_signal(&line.costumerInLine);
	pthread_mutex_lock(&line.costumers_mutex[arg_t->id]);
	while (!lineNumberCheckedOut(lineNumber)) {
		pthread_cond_wait(&line.costumers_cond,
						  &line.costumers_mutex[arg_t->id]);
	}
	pthread_mutex_unlock(&line.costumers_mutex[arg_t->id]);
	D(printf("Costumer %d checked out. Line number: %d\n", arg_t->id, lineNumber));
	return NULL;
}

void* Cashier(void* arg) {
	for (int i = 0; i < NR_COSTUMERS; ++i)
	{
		pthread_mutex_lock(&line.cashier_mutex);
		while (getCurrentLineNumber() < i || (i == 0 && getCurrentLineNumber() == 0)) {
			pthread_cond_wait(&line.costumerInLine,
							  &line.cashier_mutex);
		}
		Checkout(i);
		pthread_cond_broadcast(&line.costumers_cond);
		//sleep(1);
		pthread_mutex_unlock(&line.cashier_mutex);
	}
	return NULL;
}

int lineNumberCheckedOut(int lineNumber) {
	pthread_mutex_lock(&line.checkout_mutex[lineNumber]);
	int checkedOut = line.costumers[lineNumber];
	pthread_mutex_unlock(&line.checkout_mutex[lineNumber]);
	return checkedOut;
}

int inspectCone() {
	D(printf("%s\n", "Manager inspects cone"));
	return rand() % 2 == 0;  //50% chance of failing
};

void makeCone() {
	D(printf("%s\n", "Clerk making cone.."));
};

int getNextLineNumber() {
	pthread_mutex_lock(&line.lock);
	int lineNumber = line.currentLineNumber++;
	pthread_mutex_unlock(&line.lock);
	return lineNumber;
}

int getCurrentLineNumber() {
	pthread_mutex_lock(&line.lock);
	int lineNumber = line.currentLineNumber;
	pthread_mutex_unlock(&line.lock);
	return lineNumber;
}

void Checkout(int lineNumber) {
	pthread_mutex_lock(&line.checkout_mutex[lineNumber]);
	line.costumers[lineNumber] = 1;
	pthread_mutex_unlock(&line.checkout_mutex[lineNumber]);
	D(printf("%s%d\n", "Checking out: ",i));
}

void initInspector() {
	inspector.pass = 0;
	inspector.finished = 0;
	inspector.request = 0;
	pthread_cond_init(&(inspector.request_cond), NULL);
	pthread_cond_init(&(inspector.finished_cond), NULL);
	pthread_mutex_init(&(inspector.lock), NULL);
	pthread_mutex_init(&(inspector.inspection), NULL);
}

void initLine() {
	line.currentLineNumber = 0;
	pthread_cond_init(&line.costumerInLine, NULL);
	pthread_cond_init(&line.costumers_cond, NULL);
	for (int i = 0; i < NR_COSTUMERS; ++i)
	{
		line.costumers[i] = 0;
		pthread_mutex_init(&line.costumers_mutex[i], NULL);
		pthread_mutex_init(&line.checkout_mutex[i], NULL);
	}
	pthread_mutex_init(&line.lock, NULL);
	pthread_mutex_init(&line.cashier_mutex, NULL);
}