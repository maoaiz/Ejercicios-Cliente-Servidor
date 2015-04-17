/* Created by: Mauricio Aizaga
 * Universidad Tecnologica de Pereira
 * 
 * */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
void tables(int n);
void primes(int n);

void tables(int n){
	printf ("tablas %d\n", n);
	int num, i;
	for (num = 1; num <= n; num++){
		for (i=1; i<=10; i++)
			printf("%d x %d = %d\n",num,i,num*i);
	}
}

void primes(int n){
	int num, i, is_prime;
	printf ("primos %d\n", n);
	for (num = 1; num <= n; num++){
		is_prime = 1;
  
		for(i = 2; i <= num / 2; i = i + 1)
			if((num%i)==0) 
				 is_prime = 0;

		if(is_prime==1) 
		  printf("%d es numero primo\n", num);
		  
	}
}

int spawn (int j)
{
	pid_t child_pid;
	child_pid = fork ();
	if (child_pid != 0)
	{
		return child_pid;
	}
	else 
	{
		if (j == 1)
		{
			tables(100);
		}
		else if(j == 2)
		{
			primes(100);
		}
		return child_pid;
	}
}

int main ()
{
	pid_t child_pid;
	child_pid = spawn (1); 
	if (child_pid != 0)
	{
		spawn (2);
	}
	return 0;
}
