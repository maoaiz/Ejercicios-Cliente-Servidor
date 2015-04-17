/**
 *
 * Taller: Modificar el taller 4 para que el proceso padre imprima el
 * número que es retornado por cada hilo.
 * 
 * Created by: Mauricio Aizaga
 * Febrero 19 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

//void tables(int n);
//void primes(int n);

void* tables(void* n){
	int* p = (int*) n;
	printf ("tablas %d\n", *p);
	int num, i;
	for (num = 1; num <= *p; num++){
		for (i=1; i<=10; i++)
			printf("%d x %d = %d\n",num,i,num*i);
	}
	return (void*) 1;
}

void* primes(void* p){
	int* n = (int*) p;
	int num, i, is_prime;
	printf ("primos %d\n", *n);
	for (num = 1; num <= *n; num++){
		is_prime = 1;
  
		for(i = 2; i <= num / 2; i = i + 1)
			if((num%i)==0) 
				 is_prime = 0;

		if(is_prime==1) 
		  printf("%d es numero primo\n", num);
		  
	}
	return (void*) 1;
}

int main ()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	int* num_tables;
	int* num_primes;
	
	num_tables = NULL;
	num_tables = (int *) malloc(sizeof (int));
	*num_tables = 30;

	num_primes = NULL;
	num_primes = (int *) malloc(sizeof (int));
	*num_primes = 40;
	
	pthread_create (&thread1_id, NULL, &tables, num_tables);
	
	pthread_create (&thread2_id, NULL, &primes, num_primes);
	
	//printf("NUM: %p", num);
	void * tables_status;
	void * primes_status;
	pthread_join (thread1_id, &tables_status);
	pthread_join (thread2_id, &primes_status);

	printf("The %dth tables number was printed. Status=%d\n", *num_tables, (int) tables_status);
	printf("The %dth primes number was printed. Status=%d\n", *num_primes, (int) primes_status);
	
	free(num_tables);
	free(num_primes);
	return 0;
}
