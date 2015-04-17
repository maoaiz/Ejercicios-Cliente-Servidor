/**
 * Taller: Modificar el taller 1 para que en lugar de crear dos procesos
 * se creen dos hilos
 * 
 * Created by: Mauricio Aizaga
 * Febrero 18 de 2014
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
	return NULL;
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
	return NULL;
}

int main ()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	int* num;
	num = NULL;
	num = (int *) malloc(sizeof (int));
	*num = 30;
	
	pthread_create (&thread1_id, NULL, &tables, num);
	
	pthread_create (&thread2_id, NULL, &primes, num);
	
	printf("NUM: %p", num);
	pthread_join (thread1_id, NULL);
	pthread_join (thread2_id, NULL);
	return 0;
}
