#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <funciones/sockets-lib.h>

int main(void) {
	puts("Soy Worker"); /* prints Soy Worker */
	int unSocket = crearSocket();
	printf("socket = %d", unSocket);
	return EXIT_SUCCESS;
}
