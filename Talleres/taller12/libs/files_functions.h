
#include <dirent.h>
#include <stdio.h>

#define MAX_RESPONSE_SIZE 1000

char * show_dir_with_system(char * path){
    /*Muestra un directorio usando system*/
    char *command = (char *)malloc(sizeof(char) * 200);
    strcpy(command, "/bin/ls -Al ");
    strcat(command, path);
    strcat(command, " | awk '{print $9}' ");
    
    FILE *fp;
    char *resp = (char *)malloc(sizeof(char) * MAX_RESPONSE_SIZE);

    /* Open the command for reading. */
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    char *buf = (char *)malloc(sizeof(char) * MAX_RESPONSE_SIZE);
    strcpy(buf, "DIRECTORIO COMPARTIDO: ");
    strcat(buf, path);
    strcat(buf, "\n\n");
    /* Read the output a line at a time - output it. */
    while (fgets(resp, sizeof(resp)-1, fp) != NULL) {
        // printf("%s", resp);
        strcat(buf, resp);
    }
    /* close */
    pclose(fp);

    return buf;
}

char * show_dir(char * path){
    /*Muestra un directorio usando readdir*/
    
    DIR *my_path;
    struct dirent * dir;
    
    if ((my_path = opendir(path)) == NULL) {
        fprintf(stderr, "Error al leer el directorio\n");
        exit (1);
    }

    char *buf = (char *)malloc(sizeof(char) * 1000);
    strcpy(buf, "DIRECTORIO COMPARTIDO: ");
    strcat(buf, path);
    strcat(buf, "\n\n");
    while ((dir=readdir(my_path)) != NULL) {
        // printf("%d - %c - %s\n", dir->d_ino, dir->d_type, dir->d_name);
        strcat(buf, "\t");
        strcat(buf, dir->d_name);
        strcat(buf, "\n");
    }
    // Cerramos el directorio
    closedir(my_path);

    printf("show_dir\n");
    system("ls -l shared_files/");
    return buf;
}


char * upload_file(char * path){
    printf("upload_file: %s\n", path);
    return "upload_file.......... %s", path;
}


char * delete_file(char * path){
    printf("ELIMINANDO EL ARCHIVO: %s...............\n", path);
    char *command = (char *)malloc(sizeof(char) * 100);
    strcpy(command, "rm -f ");
    strcat(command, path);
    printf("Command executed: %s\n", command);
    system(command);
    return "Archivo borrado.";
}