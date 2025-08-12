/*
 * sockets-lib.c
 *
 *  Created on: 5/9/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int crearSocket() // funcion para crear socket
{
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) //inicializamos el socket
        {
      perror("Socket Fail");
      exit(1);
    }
    return socketfd; //devolvemos el socket creado
}

void inicializarSOCKADDR_IN(struct sockaddr_in* direccion, char* direccionIP,char* puerto) // La funcion transforma sola los datos de host a network
{
    direccion->sin_family = AF_INET;
    direccion->sin_addr.s_addr = inet_addr(direccionIP);
    direccion->sin_port = htons(atoi(puerto));
    memset(&(direccion->sin_zero), '/0', 8);
    return;
}

void asignarDirecciones(int socketFD, const struct sockaddr* sockDIR) //Asociamos el puerto y direccion al socket
{
    if (bind(socketFD, sockDIR, sizeof(struct sockaddr)) == -1) {
      perror("Bind fail");
      exit(3);
    }
    return;
}

void reutilizarSocket(int socketFD) {
    int yes = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) //el socket se puede reutilizar
        {
      perror("setsockopt");
      exit(2);
    }
    return;
}
