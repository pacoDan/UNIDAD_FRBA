/*
 * sockets-lib.h
 *
 *  Created on: 5/9/2017
 *      Author: utnso
 */

#ifndef FUNCIONES_SOCKETS_LIB_H_
#define FUNCIONES_SOCKETS_LIB_H_

int crearSocket();

void inicializarSOCKADDR_IN(struct sockaddr_in*, char*, char*);

void asignarDirecciones(int, const struct sockaddr*);

void reutilizarSocket(int);

#endif /* FUNCIONES_SOCKETS_LIB_H_ */
