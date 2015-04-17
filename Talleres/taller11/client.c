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
 * PROGRAMA CLIENTE:
 *   Para correr el compilado: ./client.o DIRECCION_IP_DEL_SERVIDOR
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
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3490 // puerto al que vamos a conectar 

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez 

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    char *command = (char *)malloc(sizeof(char) * MAXDATASIZE);
    struct hostent *he;
    struct sockaddr_in their_addr; // información de la dirección de destino 
	char op[1], str1[10], str2[10];
	//printf("argc: %d", argc);

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {  // obtener información de máquina 
        perror("gethostbyname");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&(their_addr),0,sizeof(their_addr));
    their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina 
    their_addr.sin_port = htons(PORT);  // short, Ordenación de bytes de la red 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    //memset(&(their_addr.sin_zero), 8);  // poner a cero el resto de la estructura 

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("Conected: %s", buf);
    //printf("comando enviado: %s\n", argv[2]);
    //id_operacion#num1#num2
    
    printf("****************************\n");
    printf("    Escoja una opción\n\n");
    printf("1)Suma\n2)Resta\n3)Multiplicación\n4)División\n");
    printf("\nSu opción:");
    scanf("%s", &op);
    printf("\nIngrese valor 1: ");
    scanf("%s", &str1);
    printf("Ingrese valor 2: ");
    scanf("%s", &str2);
    printf("\n");
    
    strcat(command, op);
    strcat(command, "#");
    strcat(command, str1);
    strcat(command, "#");
    strcat(command, str2);
    //command = "1#2#3\0";
    //printf("comando enviado: %s\n", command);
    
    if (send(sockfd, command, 14, 0) == -1)
        perror("send");

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

	switch(atoi(op)){
        case 1: printf("Operación: Suma\n");
            break;
        case 2: printf("Operación: Resta\n");
            break;
        case 3: printf("Operación: multiplicacion\n");
            break;
        case 4: printf("Operación: Divición\n");
            break;
        default: printf("[ERROR] no se especificó una operación válida\n");
    }

    printf("Respuesta: %s\n",buf);
    
    close(sockfd);

    return 0;
} 

