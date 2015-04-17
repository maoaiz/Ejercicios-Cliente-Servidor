/**
 * Taller 9: Trabajo con Variables de condición
 * 
 * 
 * Created by: Mauricio Aizaga
 * Marzo 05 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


int thread_flag;
pthread_cond_t thread_flag_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t thread_flag_mutex = PTHREAD_MUTEX_INITIALIZER;

struct job {
	struct job* next;
	int num;
};

/* A linked list of pending jobs.*/
struct job* job_queue;
struct job* tail;


void initialize_flag ()
{
	/* Initialize the mutex and condition variable.*/
	//pthread_mutex_init (&thread_flag_mutex, NULL);
	//pthread_cond_init (&thread_flag_cv, NULL);
	/* Initialize the flag value. */
	thread_flag = 0;
	printf("thread_flag: %d\n", thread_flag);
}


/* Sets the value of the thread flag to FLAG_VALUE.*/
void set_thread_flag (int flag_value)
{
	/* Lock the mutex before accessing the flag value. */
	pthread_mutex_lock (&thread_flag_mutex);
	/* Set the flag value, and then signal in case thread_function is
	blocked, waiting for the flag to become set. However,
	thread_function can’t actually check the flag until the mutex is
	unlocked. */

	thread_flag = flag_value;
	pthread_cond_signal (&thread_flag_cv);
	/* Unlock the mutex. */
	pthread_mutex_unlock (&thread_flag_mutex);
}


void print_list(struct job * job_list){
	int i;
	i=1;
	struct job* aux = job_queue;
	while(aux != NULL){
		printf("%d) %d\n",i,aux->num);
		aux = aux->next;
		i++;
	}
	printf("Terminó la lista.\n\n");
}

void init_list (int list_lentgh){
	int i, n;
	srand(time(NULL));

	struct job* aux = job_queue;
	for(i = 0; i < list_lentgh; i++){
		n = rand()%100;
		aux->num = n;
		tail = NULL;
		tail = (struct job *) malloc(sizeof (struct job));
		/* Lock the mutex on the job queue before accessing it.*/
		pthread_mutex_lock (&thread_flag_mutex);
		
		aux->next = tail;
		aux = aux->next;
		
		
		pthread_mutex_unlock (&thread_flag_mutex);
	}
	set_thread_flag(1);
}

char* is_prime(int a){
	int c;
 
    for ( c = 2 ; c <= a - 1 ; c++ )
      if ( a%c == 0 )
	 	return "No";
    if ( c == a )
		return "Si";
	return NULL;
}

int process_job(struct job* job_obj, int option){
	// printf("OPCION: %d\t", option);
	switch(option){
		case 1: printf("La raiz cuadrada de %d es: %.3f\n",job_obj->num, sqrt(job_obj->num));break;
		case 2: printf("El logaritmo natural de %d es: %.3f\n",job_obj->num, log(job_obj->num));break;
		case 3: printf("%d es un numero primo? %s\n", job_obj->num, is_prime(job_obj->num));break;
	}
	return 0;
}
int do_work(int n){
	set_thread_flag(0);
	printf("trabajo del hilo %d\n\n", n);
	sleep(1);
	//process_job();
	set_thread_flag(1);
	return 0;
}

/* Calls do_work repeatedly while the thread flag is set; blocks if
the flag is clear. */

void* thread_function (void* arg){
	int* n = (int *) arg;
	printf("thread_flag before while: %d (hilo %d)\n", thread_flag, *n);
	while (1) {
		pthread_mutex_lock (&thread_flag_mutex);
		while (!thread_flag){
			pthread_cond_wait (&thread_flag_cv, &thread_flag_mutex);
			printf("hilo %d esperando...\n", *n);
		}
		pthread_mutex_unlock (&thread_flag_mutex);
		printf("Hilo %d ejecutando\n", *n);
		do_work (*n);
	}
	return (void*) 1;
}


int main(){
	printf("Iniciando...\n");
	job_queue = NULL;
	job_queue = (struct job *) malloc(sizeof (struct job));
	initialize_flag();

	init_list(9);
	print_list(job_queue);
	pthread_t thread1_id;
	pthread_t thread2_id;
	pthread_t thread3_id;

	int* num;
	num = NULL;
	num = (int *) malloc(sizeof (int));
	*num = 1;
	pthread_create (&thread1_id, NULL, &thread_function, num);
	
	int* num2;
	num2 = NULL;
	num2 = (int *) malloc(sizeof (int));
	*num2 = 2;
	pthread_create (&thread2_id, NULL, &thread_function, num2);
	
	int* num3;
	num3 = NULL;
	num3 = (int *) malloc(sizeof (int));
	*num3 = 3;
	pthread_create (&thread3_id, NULL, &thread_function, num3);


	void * status1;
	void * status2;
	void * status3;
	pthread_join (thread1_id, &status1);
	pthread_join (thread2_id, &status2);
	pthread_join (thread3_id, &status3);
	
	free(num);
	free(num2);
	free(num3);
	printf("FInal thread_flag: %d\n", thread_flag);
	return 0;
}

