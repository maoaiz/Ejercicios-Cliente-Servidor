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

/******SERVER*****/

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
#include "libs/chars_manager.h"
#include "libs/files_functions.h"


#define MYPORT 3490    // Puerto al que conectarán los usuarios
#define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola
#define MAX_RESPONSE_SIZE 1000

void sigchld_handler(int s){
    while(wait(NULL) > 0);
}

char * request(char buf[]){
    printf("\n***************<REQUEST>********************\n");
    printf("El cliente solicita: %s\n", buf);

    /* <Decodificar el comando enviado desde el cliente> */
    int i;
    char * p;
    char *array[3];
    i = 0;
    p = strtok(buf,"#"); 
    while (p != NULL){
        array[i++] = p;
        p = strtok (NULL, "#");
    }
    /* </Decodificar el comando enviado desde el cliente> */

    char *resp = (char *)malloc(sizeof(char) * MAX_RESPONSE_SIZE);

    printf("resto::%d\n", atoi(array[0]));

    switch(atoi(array[0])){
        case 0: printf("El cliente quiere irse...\n");
            resp = "exit";
            // return resp;
            break;
        case 1: printf("Operación: listar directorio\n");
            resp = show_dir("shared_files");
            // resp = show_dir_with_system("shared_files/");
            break;
        case 2: printf("Operación: subir archivo\n");
            resp = upload_file(array[1]);
            break;
        case 3: printf("Operación: eliminar archivo\n");
            // strcpy(resp, "shared_files/");
            // strcat(resp, array[1]);
            // delete_file(resp);
            resp =  "Archivo borrado";
            break;
        default: printf("[ERROR] no se especificó una operación válida\n");
            resp = "error";
            return resp;
    }
    printf("******************</REQUEST>***************\n");
    // free(array[0]);
    // free(array[1]);
    return resp;
}

int main(void){
    int sockfd, new_fd, numbytes;  // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
    struct sockaddr_in my_addr;    // información sobre mi dirección
    struct sockaddr_in their_addr; // información sobre la dirección del cliente
    int sin_size;
    struct sigaction sa;
    int yes=1;

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
            int b;
            b = 1;
            char buf[MAXDATASIZE];
            char *response = (char *)malloc(sizeof(char) * MAX_RESPONSE_SIZE);
            response = "Null";
            while(b == 1){
                printf("Escuchando al cliente...\n%s", response);
                if ((numbytes=recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
                    perror("recv");
                    exit(1);
                }
                buf[numbytes] = '\0';
                printf("COMANDO SOLICITADO: %s\n", buf);

                response = request(buf);
                if (response == "exit"){
                    b = 0;
                    break;
                }
                if (response == "error"){
                    response = "[ERROR] No existe esa operación\0";
                }

                // printf("Response: %s\n", response);
                if (send(new_fd, response, MAX_RESPONSE_SIZE-1, 0) == -1)
                    perror("send");
                response = "";
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // El proceso padre no lo necesita
    }

    return 0;
}