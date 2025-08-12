#ifndef FM9_H_
#define FM9_H_

#include <qcommons/console.h>
#include <qcommons/serialize.h>
#include <qcommons/socket.h>
#include <qcommons/protocolos.h>
#include <pthread.h>

#include "config/config.h"
#include "consola/consola.h"
#include "storage/storage.h"

#define FILE_CONFIG_FM9 "fm9.config"
#define FM9 "FM9"
#define TAMANIO_CANT_CLIENTES 10

int socket_fm9;

pthread_t hilo_cliente;
pthread_t hilo_consola;
pthread_t hilo_principal;

int inicializar();
void escuchar_consola();
void iniciar_fm9();
void crear_servidor();
void atender_conexiones();
void administrar_servidor(void *puntero_fd);
void iniciar_administracion_memoria();
void liberar_recursos(int tipo_salida);
void terminar_exitosamente();
void coordinarDAM(int socket, void* datos, header_paquete* paquete);
void coordinarCPU(int socket, void* datos, header_paquete* paquete);
int recibir_datos(void* paquete, int socketFD, uint32_t cant_a_recibir);

#endif /* FM9_H_ */
