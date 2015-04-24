/**
 * Taller: Desarrollar un sistema C/S para compartir archivos con las siguientes características:
 *  
 *  Servidor:
 *    Debe soportar varios clientes simultáneos
 *    Debe compartir todos los archivos de texto en una carpeta predeterminada
 *    La conexión con cada cliente debe permanecer abierta hasta que el cliente la cierra de manera explícita.
 *  
 *  Cliente: Debe solicitar los siguientes servicios:
 *    Lista de archivos
 *    Envío de un archivo determinado
 *    Borrar un archivo

 *  
 * Created by: Mauricio Aizaga
 * Abril 21 de 2014
 * github.com/maoaiz
 * Universidad Tecnologica de Pereira
 * */

/******CLIENT*****/


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


char * get_file_name_to_delete(char * buf){
    /* En buf está el resultado de solicitar listar archivos.
    Se imprime el listado de archivos para mostrar cuales son
    los posibles archivos a eliminar */
    printf("%s\n", buf);

    printf("Escriba el nombre del archivo a eliminar: ");
    char *name = (char *)malloc(sizeof(char) * 30);
    scanf("%s", name);

    printf("El archivo a borrar es: %s\n\n", name);
    return name;
}


int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    char *command = (char *)malloc(sizeof(char) * MAXDATASIZE);
    struct hostent *he;
    struct sockaddr_in their_addr; // información de la dirección de destino 
    char op[1], str1[10];
    char opt[1];
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
    //id_operacion#nombre-archivo-a-eliminar
    
    printf("****************************\n");
    printf("    Escoja una opción\n\n");
    printf("1)Listar archivos\n2)Subir archivo\n3)Eliminar archivo\n");
    printf("\nSu opción:");
    scanf("%s", &op);
    printf("\n");


    if (atoi(op) == 2 ){
        printf("\nIngrese la ruta del archivo: ");
        scanf("%s", &str1);
    }else{
        strcpy(str1, "-");
        if (atoi(op) == 3 ){
            strcpy(opt, "1");
            // printf("op ==3; %s\n", op);
        }else{
            strcpy(opt, op);
            // printf("else; %s\n", op);
        }
    }
    strcat(command, opt);
    strcat(command, "#");
    strcat(command, str1);
    printf("comando enviado: %s\n", command);
    
    printf("solicitud 1\n");
    if (send(sockfd, command, 14, 0) == -1){
        perror("send");
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
    printf("BUF:%s\n",buf);

    // printf("\n\nsolicitud 2\n");
    // if (send(sockfd, command, 14, 0) == -1){
    //     perror("send");
    // }

    // if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    //     perror("recv");
    //     exit(1);
    // }

    buf[numbytes] = '\0';
    printf("%s\n",buf);

    switch(atoi(op)){
        case 1: printf("Lista de archivos:\n");
            printf("%s\n",buf);
            break;
        case 2: printf("Archivo subido correctamente\n");
            printf("%s\n",buf);
            break;
        case 3: printf("Listado de Archivos a Eliminar\n");
            char *name2 = (char *)malloc(sizeof(char) * 30);
            name2 = get_file_name_to_delete(buf);


            char *command2 = (char *)malloc(sizeof(char) * MAXDATASIZE);
            strcat(command2, "3#");
            strcat(command2, name2);


            if (send(sockfd, command2, 50, 0) == -1){
                perror("send");
            }

            printf("COMANDO ENVIADO: %s\n", command2);

            if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                perror("recv");
                exit(1);
            }
            buf[numbytes] = '\0';
            printf("El archivo %s ha sido borrado. \n", name2);
            printf("%s\n", buf);

            break;
        default: printf("[ERROR] no se especificó una operación válida\n");
    }
    close(sockfd);

    return 0;
} 

