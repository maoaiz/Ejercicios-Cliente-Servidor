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

#define MAX_RESPONSE_SIZE 1000

char * get_file_name_to_delete(char * filename_list){
    /* En buf está el resultado de solicitar listar archivos.
    Se imprime el listado de archivos para mostrar cuales son
    los posibles archivos a eliminar */
    printf("%s\n", filename_list);

    printf("Escriba el nombre del archivo a eliminar: ");
    char *name = (char *)malloc(sizeof(char) * 30);
    scanf("%s", name);

    printf("\n\aArchivo a borrar: %s\n\n", name);
    return name;
}

char * show_menu(){
    char *op = (char *)malloc(sizeof(char) * 10);

    printf("\n\n***************************\n");
    printf("    Escoja una opción\n\n");
    printf("0) Salir\n1) Listar archivos\n2) Subir archivo\n3) Eliminar archivo\n");
    printf("\nSu opción: ");
    scanf("%s", op);
    printf("\n");
    return op;
}

char * build_command(char *op){
    /* Estructura del comando a construir:
        id_operacion#nombre-archivo-a-eliminar*/
    char *command = (char *)malloc(sizeof(char) * MAXDATASIZE);
    char *opt = (char *)malloc(sizeof(char) * 10);
    if (atoi(op) == 3){
        strcpy(opt, "1");
    }else{
        strcpy(opt, op);
    }
    strcpy(command, opt);
    strcat(command, "#-");
    // printf("comando enviado: %s\n", command);
    return command;
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // información de la dirección de destino 

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

    while (1){
        char *command = (char *)malloc(sizeof(char) * MAXDATASIZE);
        char *op = (char *)malloc(sizeof(char) * 10);
        char *buf = (char *)malloc(sizeof(char) * MAX_RESPONSE_SIZE);

        op = show_menu();
        command = build_command(op);

        /*Se envía el comando correspondiente a la operación solicitada*/
        if (send(sockfd, command, MAXDATASIZE-1, 0) == -1){
            perror("send");
        }

        /*Se recibe la respuesta de la operación solicitada*/
        if ((numbytes=recv(sockfd, buf, MAX_RESPONSE_SIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';

        /*Se analíza la operación solicitada para mostrar su resultado*/
        switch(atoi(op)){
            case 0: printf("Chao mundo\n");
                exit(0);
                break;
            case 1: printf("Lista de archivos:\n");
                printf("%s\n", buf);
                break;
            case 2: printf("Contenido del Archivo\n");
                printf("%s\n", buf);
                break;
            case 3: printf("Listado de Archivos a Eliminar\n");
                char *name2 = (char *)malloc(sizeof(char) * 30);
                name2 = get_file_name_to_delete(buf);


                char *command2 = (char *)malloc(sizeof(char) * MAXDATASIZE);
                strcpy(command2, "3#");
                strcat(command2, name2);


                if (send(sockfd, command2, 50, 0) == -1){
                    perror("send");
                }

                // printf("SEGUNDO COMANDO ENVIADO: %s\n", command2);

                if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                    perror("recv");
                    exit(1);
                }
                buf[numbytes] = '\0';
                // printf("El archivo %s ha sido borrado. \n", name2);
                printf("%s\n", buf);

                break;
            default: printf("[ERROR] no se especificó una operación válida\n");
        }
        free(command);
        free(op);
    }
    free(buf);
    close(sockfd);

    return 0;
} 

