/**
 * Taller: Modificar el ejercicio Listing 4.10, llenar una lista con
 * enteros y manejar 3 hilos donde cada uno saca un elemento de la
 * lista y relaliza la siguiente operación:
 * 
 * hilo 1: Raiz cuadrada
 * hilo 2: Logaritmo natural
 * hilo 3: Que diga si es primo o no
 * 
 * 
 * Created by: Mauricio Aizaga
 * Febrero 26 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>


struct job {
	struct job* next;
	int num;
};
/* A linked list of pending jobs.*/
struct job* job_queue;
struct job* tail;


void print_list(struct job * job_list){
	int i;
	i=1;
	struct job* aux = job_queue;
	while(aux != NULL){
		printf("%d) %d\n",i,aux->num);
		aux = aux->next;
		i++;
	}
}

char* is_prime(int a){
	int c;
 
    for ( c = 2 ; c <= a - 1 ; c++ )
    { 
      if ( a%c == 0 )
	 	return "No";
    }
    if ( c == a )
		return "Si";
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


/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_function (void* arg){
	int* n = (int *) arg;
	printf("*********************Soy el hilo %d**************************\n", *n);
	while (1) {
		struct job* next_job;
		/* Lock the mutex on the job queue. */
		pthread_mutex_lock (&job_queue_mutex);
		/* Now it’s safe to check if the queue is empty. */
		if (job_queue == NULL)
			next_job = NULL;
		else {
			/* Get the next available job. */
			next_job = job_queue;
			/* Remove this job from the list. */
			job_queue = job_queue->next;
		}
		/* Unlock the mutex on the job queue because we’re done with the
		queue for now. */
		pthread_mutex_unlock (&job_queue_mutex);
		/* Was the queue empty? */
		if (next_job == NULL)
			break;
		/*If so, end the thread.*/
		/* Carry out the work. */
		process_job (next_job, *n);
		/* Clean up. */
		free (next_job);
	}
	return (void*) 1;
}


int main (int argc, char *argv[])
{
	int list_lentgh;
	if (argc >= 2){
		list_lentgh = atoi(argv[1]);
    	printf("%s\n", argv[1]);
    }else{
    	list_lentgh = 100; //Por defecto la lista tendrá 100 elementos
    }

	job_queue = NULL;
	job_queue = (struct job *) malloc(sizeof (struct job));
	
	int i, n;
	srand(time(NULL));

	struct job* aux = job_queue;
	for(i = 0; i < list_lentgh; i++){
		n = rand()%100;
		aux->num = n;
		tail = NULL;
		tail = (struct job *) malloc(sizeof (struct job));
		aux->next = tail;
		aux = aux->next;
	}
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
	// printf("The %dth tables number was printed. Status=%d\n", *num, (int) status1);
	
	pthread_join (thread2_id, &status2);
	pthread_join (thread3_id, &status3);
	
	free(num);
	return 0;
}
