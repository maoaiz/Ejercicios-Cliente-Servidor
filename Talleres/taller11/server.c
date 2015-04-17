/**
 * Taller: Hacer un programa cliente y uno servidor que realice las 
 * siguientes tareas:
 *    Cliente: Solicita realizar alguna de las siguientes operaciones
 *            en el servidor:
 *				suma
 *				resta
 *				mult
 *				división
 *    Servidor: realiza las operaciones y envía la respuesta.
 *
 * PROGRAMA SERVIDOR:
 *   Para correr el compilado: ./server.o
 *  
 * Created by: Mauricio Aizaga
 * Marzo 24 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

#define MYPORT 3490    // Puerto al que conectarán los usuarios
#define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola
#define MAXDATASIZE 100 

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
**/
void strreverse(char* begin, char* end) {
    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}

static void itoa(intptr_t value, char* str, int base) {
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char* wstr=str;
	intptr_t sign;
	div_t res;
	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return; }
	// Take care of sign
	if ((sign=value) < 0) value = -value;
	// Conversion. Number is reversed.
	do {
		res = div(value, base);
		*wstr++ = num[res.rem];
	} while((value=res.quot));
	if (sign < 0) *wstr++='-';
	*wstr = '\0';
	// Reverse string
	strreverse(str, wstr - 1);
}


/*************************************************************************
*
* Converting float to string without using library function like sprintf
* (naive approach , may lead to anomalies)
*
* by Mayank Bhola
* Taked from: https://gist.github.com/psych0der/6319244
*
**************************************************************************/
#define precision 6  //precision for decimal digits

char* ftoa(float f){
        
    float ff;
    //scanf("%f",&f);
    ff = f;
    //char str[30];

    char *str = (char *)malloc(sizeof(char) * MAXDATASIZE);
    int a,b,c,k,l=0,m,i=0,j;
    
    // check for negetive float
    if(f<0.0)
    {
        
        str[i++]='-';
        f*=-1;
    }
    
    a=f;    // extracting whole number
    f-=a;   // extracting decimal part
    k = precision;
    
    // number of digits in whole number
    while(k>-1)
    {
        l = pow(10,k);
        m = a/l;
        if(m>0)
        {
            break;
        }
    k--;
    }

    // number of digits in whole number are k+1
    
    /*
    extracting most significant digit i.e. right most digit , and concatenating to string
    obtained as quotient by dividing number by 10^k where k = (number of digit -1)
    */
    
    for(l=k+1;l>0;l--)
    {
        b = pow(10,l-1);
        c = a/b;
        str[i++]=c+48;
        a%=b;
    }
    str[i++] = '.';
    
    /* extracting decimal digits till precision */

    for(l=0;l<precision;l++)
    {
        f*=10.0;
        b = f;
        str[i++]=b+48;
        f-=b;
    }
    str[i]='\0';
    return (str);
}


/**********************************************************************************/

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}



char * request(char buf[]){
    printf("\n*****************************************\n");
    printf("El cliente solicita: %s\n", buf);
    int i, ans;
    char * p;
    char *resp = (char *)malloc(sizeof(char) * MAXDATASIZE);
    char *array[3];
    i = 0;
    p = strtok(buf,"#");  
    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "#");
    }
        //hacer calculos
        //printf ("a: %d", atoi(array[1]));
    float n;
    switch(atoi(array[0])){
        case 1: printf("Operación: suma\n");
            n = atof(array[1]) + atof(array[2]);
            break;
        case 2: printf("Operación: resta\n");
            n = atof(array[1]) - atof(array[2]);
            break;
        case 3: printf("Operación: mult\n");
            n = atof(array[1]) * atof(array[2]);
            break;
        case 4: printf("Operación: div\n");
            n = atof(array[1]) / atof(array[2]);
            break;
        default: printf("[ERROR] no se especificó una operación válida\n");
        resp = "error";
        return resp;
    }
    printf("Num 1: %s\n", array[1]);
    printf("Num 2: %s\n", array[2]);


    printf("Resultado: %f\n", n);
    resp = ftoa(n);
    //printf("Resultado (s): %s\n", resp);
    printf("*****************************************\n");
    return resp;
}

int main(void)
{
    int sockfd, new_fd, numbytes;  // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
    struct sockaddr_in my_addr;    // información sobre mi dirección
    struct sockaddr_in their_addr; // información sobre la dirección del cliente
    int sin_size;
    struct sigaction sa;
    int yes=1;
    char buf[MAXDATASIZE];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;         // Ordenación de bytes de la máquina
    my_addr.sin_port = htons(MYPORT);     // short, Ordenación de bytes de la red
    my_addr.sin_addr.s_addr = INADDR_ANY; // Rellenar con mi dirección IP
    memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // Eliminar procesos muertos
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {  // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) 
        {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
        if (!fork()) { // Este es el proceso hijo
            close(sockfd); // El hijo no necesita este descriptor
            if (send(new_fd, "Bienvenido!\n", 14, 0) == -1)
                perror("send");

            if ((numbytes=recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
				perror("recv");
				exit(1);
            }

            buf[numbytes] = '\0';

            char *resp2 = request(buf);
            if (resp2 == "error"){
                printf("resp2: %s\n", resp2);
                resp2 = "[ERROR] No existe esa operación\0";
            }
            if (send(new_fd, resp2, MAXDATASIZE-1, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // El proceso padre no lo necesita
    }

    return 0;
} 


//https://gist.github.com/psych0der/6319244 Sin probar
