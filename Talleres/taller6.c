/**
 * Taller: Crear un programa que permita de manera aleatoria, simular
 * transacciones entre cuentas utilizando hilos
 * 
 * hilo 1: Llena los datos de las cuentas
 * hilo 2: realiza transacciones aleatorias entre cuentas
 * 
 * Created by: Mauricio Aizaga
 * Febrero 24 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


float* account_balances;

void print_accounts(float* acc){
	int i;
	for (i = 0; i<20; i++){
		printf("%d) %.3f\n",i, acc[i]);
	}
}

int process_transaction (int from_acct, int to_acct, float dollars)
{
	printf("Se va a transferir %.2f de la cuenta %d a la cuenta %d\n", dollars, from_acct, to_acct );
	int old_cancel_state;
	/* Check the balance in FROM_ACCT. */
	if (account_balances[from_acct] < dollars)
		return 1;
	/* Begin critical section. */
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);
	/* Move the money. */
	account_balances[to_acct] += dollars;
	account_balances[from_acct] -= dollars;
	/* End critical section. */
	pthread_setcancelstate (old_cancel_state, NULL);
	return 0;
}

void* fill_data(void* parameters){
	int i;
	srand(time(NULL));
	for (i = 0; i<20; i++){
		account_balances[i] = rand()%100;
	}
	print_accounts(account_balances);
	return (void *) 0;
}

void* make_transations(void* parameters){
	int i, t, f;
	float val;
	srand(time(NULL));
	for(i=0; i<10;i++){
		f = rand() % 20;
		t = rand() % 20;
		val = rand() % 3;
		process_transaction(f, t, val);
	}
	
	print_accounts(account_balances);
	return (void*) 0;
}


int main ()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	
	account_balances = NULL;
	account_balances = (float *) malloc(sizeof (float)*20);
	
	pthread_create (&thread1_id, NULL, &fill_data, 0);
	
	pthread_create (&thread2_id, NULL, &make_transations, 0);
	
	pthread_join (thread1_id, NULL);
	pthread_join (thread2_id, NULL);
	return 0;
}
