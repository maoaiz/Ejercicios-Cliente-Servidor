/**
 * Taller: Modificar el taller 2, para que utilizando señales, el 
 * proceso padre imprima el código de salida de los dos procesos hijos
 * 
 * Created by: Mauricio Aizaga
 * Febrero 17 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* #include <sys/types.h> */
#include <sys/wait.h>

sig_atomic_t child_exit_status;

void clean_up_child_process (int signal_number)
{
	/* Clean up the child process. */
	int status;
	wait (&status);
	/* Store its exit status in a global variable. */
	child_exit_status = status;
	printf ("estado: %d", child_exit_status);
}

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
	/* Handle SIGCHLD by calling clean_up_child_process. */
	struct sigaction sigchld_action;
	memset (&sigchld_action, 0, sizeof (sigchld_action));
	sigchld_action.sa_handler = &clean_up_child_process;
	sigaction (SIGCHLD, &sigchld_action, NULL);
	
	printf ("Soy el padre: %d\n\n", (int) getpid());
	char* arg_list[] = {
		"ls", /* argv[0], the name of the program. */
		"-l",
		NULL /* The argument list must end with a NULL. */
	};
	
	pid_t pid1, pid2;
	pid1 = spawn ("ls", arg_list);
	printf ("Soy el hijo 1: %d\n", (int) pid1);
	if (pid1 != 0){//es el padre
		char* arg_list[] = {
			"ps", 
			"-l",
			NULL
		};
		sleep(10);
		pid2 = spawn ("ps", arg_list);
		printf ("Soy el hijo 2: %d\n", (int) pid2);

		if (pid2 != 0){//sigue siendo el padre
			sleep(10);
			//kill (pid1, SIGTERM);
			//kill (pid2, SIGTERM);
		}
	}
	return 0;
}
