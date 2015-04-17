/**
 * Utilizando las funciones fork y exec, hacer un programa que llame a
 * los procesos ls y ps, luego de 1 segundo, el proceso padre debe
 * eliminar a ls dos procesos hijos * 
 *
 * Created by: Mauricio Aizaga
 * Febrero 12 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int spawn (char* program, char** arg_list)
{
	pid_t pid;
	pid = fork ();
	if (pid != 0){
		return pid;
	}
	else{
		execvp (program, arg_list);
		/* The execvp function returns only if an error occurs. */
		fprintf (stderr, "an error occurred in execvp\n");
		abort ();
	}
}

int main ()
{
	printf ("Soy el padre: %d\n\n", (int) getpid());
	char* arg_list[] = {
		"ls", /* argv[0], the name of the program. */
		"-l",
		NULL /* The argument list must end with a NULL. */
	};
	/* Spawn a child process running the “ls” command. Ignore the
	returned child process ID. */
	
	pid_t pid1, pid2;
	pid1 = spawn ("ls", arg_list);
	printf ("Soy el hijo 1: %d\n", (int) pid1);
	if (pid1 != 0){//es el padre
		char* arg_list[] = {
			"ps", 
			"-l",
			NULL
		};		
		pid2 = spawn ("ps", arg_list);
		printf ("Soy el hijo 2: %d\n", (int) pid2);
		
		if (pid2 != 0){
			sleep(1);
			int signal1_sent, signal2_sent;
			signal1_sent = kill (pid1, SIGTERM);
			signal2_sent = kill (pid2, SIGTERM);
			
			//spawn ("ps", arg_list); // Visualizar si los procesos fueron eliminados
			if (signal1_sent == 0 && signal2_sent == 0){
				printf ("Done! Process was killed");
			}else{
				printf ("The signals to kill the process couldn'd be sent.");
			}
		}
	}
	return 0;
}
