#ifndef SAFA_H_
#define SAFA_H_

#include <qcommons/console.h>
#include <qcommons/serialize.h>
#include <qcommons/socket.h>
#include <qcommons/protocolos.h>
#include <semaphore.h>
#include <pthread.h>

#include "config/config.h"
#include "consola/gestorGDT.h"
#include "planificacion/planificacion.h"
#include "commons/commons.h"
#include "cpu/cpu.h"
#include "dtb/dtb.h"

#define FILE_CONFIG_SAFA "safa.config"
#define SAFA "SAFA"
#define TAMANIO_CANT_CLIENTES 3






typedef enum {
	CORRUPTO , INICIALIZADO
} safa_status_enum;


int socket_safa;

safa_status_enum status_safa = CORRUPTO;
bool cpu_conectado=false;
bool dam_conectado=false;

pthread_t hilo_dam;
pthread_t hilo_plp;
pthread_t hilo_pcp;
pthread_t hilo_cliente;
pthread_t hilo_consola;
pthread_t hilo_principal;
pthread_t hilo_planificacion;

void inicializar_dummy( dtb_struct *dtb );

int inicializar();
void inicializar_semaforos();
void escuchar_consola();
void iniciar_safa();
void verificar_estado();
bool puede_iniciar_safa();
void crear_servidor();
void atender_conexiones();
void *administrar_servidor(void *puntero_fd);
void atender_cliente_cpu( int *socket_cliente );
void atender_cliente_dam( int *socket_cliente );
void liberar_recursos(int tipo_salida);
void terminar_exitosamente(int valor_retornado);
void escuchar_dam();


#endif /* SAFA_H_ */
