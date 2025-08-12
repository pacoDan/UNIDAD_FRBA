#include "FM9.h"

/**
 * El proceso memoria cumplira la funcion de disponibilizar
 * los datos requeridos para la ejecución de cualquier G.DT.
 * Cada vez que un G.DT requiera consultar o actualizar información
 * siempre deberá existir previamente en la Memoria.
 * */

int main(int argc, char *argv[]) {
	if (inicializar() < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();
	iniciar_administracion_memoria();
	pthread_create(&hilo_principal, NULL, (void*) iniciar_fm9, NULL);
	pthread_create(&hilo_consola, NULL, (void*) escuchar_consola, NULL);

	pthread_join(hilo_consola, NULL);
	pthread_cancel(hilo_principal);
	liberar_recursos(EXIT_SUCCESS);
	return 0;
}

int inicializar() {
	if (crear_log() == EXIT_FAILURE)
		terminar_exitosamente(EXIT_FAILURE);

	print_header(FM9, fm9_log);

	if (cargar_archivo_config(FILE_CONFIG_FM9) < 0) {
		return -1;
	}

	return 0;
}

void escuchar_consola() {
	log_info(fm9_log, "Se inicio hilo con la consola");

	while (true) {
		if (consola_leer_comando(fm9_log) == CONSOLA_TERMINAR) {
			pthread_exit(0);
			return;
		}
	}
}

void iniciar_fm9() {
	log_info(fm9_log, "Se inicio hilo principal FM9");
	crear_servidor();
	log_info(fm9_log, "Esperando por conexiones entrantes...");
	atender_conexiones();
	pthread_exit(0);
}

void crear_servidor() {
	if (configurar_socket_servidor(&socket_fm9, "127.0.0.1", fm9.puerto,
	TAMANIO_CANT_CLIENTES) < 0) {
		log_error(fm9_log, "No se pudo iniciar el servidor");
		terminar_exitosamente(EXIT_FAILURE);
	}
}

void atender_conexiones() {
	int socket_cliente;
	while ((socket_cliente = aceptar_conexion(socket_fm9))) {
		log_info(fm9_log, "Se agrego una nueva conexión, socket: %d",
				socket_cliente);

		pthread_create(&hilo_cliente, NULL, (void*) administrar_servidor,
				(void*) &socket_cliente);
	}
	if (socket_cliente < 0) {
		log_error(fm9_log, "Error al aceptar nueva conexión");
	}
}

void administrar_servidor(void *puntero_fd) {
	int socket_actual = *(int*) puntero_fd;
	header_paquete* paquete = malloc(sizeof(header_paquete));
	void* datos = NULL;
	int datos_a_recibir = 0;
	puts("administrar_servidor");
	while ((recv(socket_actual, paquete, sizeof(header_paquete), MSG_WAITALL))
			> 0) {
		//datos = paquete->mensaje;
		switch (paquete->tipo_instancia) {
		case CPU:
			coordinarCPU(socket_actual, datos, paquete);
			break;
		case DAM:
			coordinarDAM(socket_actual, datos, paquete);
			break;
		default:
			break;
		}
	}

	close(socket_actual);
	free(puntero_fd);
}

int recibir_datos(void* paquete, int socketFD, uint32_t cant_a_recibir) {
	void* datos = malloc(cant_a_recibir);
	int recibido = 0;
	int total_recibido = 0;
	do {
		recibido = recv(socketFD, datos + total_recibido,
				cant_a_recibir - total_recibido, 0);
		total_recibido += recibido;
	} while (total_recibido != cant_a_recibir && recibido > 0);
	memcpy(paquete, datos, cant_a_recibir);
	free(datos);
	if (recibido < 0) {
		printf("Socket %d desconectado\n", socketFD);
		close(socketFD);
	} else if (recibido == 0) {
		printf("Fin de conexion en socket %d\n", socketFD);
		close(socketFD);
	}
	return recibido;
}

void coordinarDAM(int socket, void* datos, header_paquete* paquete) {
	switch (paquete->tipo_operacion) {
	int pid;
	char *linea;
	case HANDSHAKE:
		if ((send(socket, (void*) &fm9.max_linea, sizeof(fm9.max_linea), 0))
				!= sizeof(fm9.max_linea)) {
			log_error(fm9_log, "¡No se pudo devolver el handshake al DAM!");
			close(socket);
		} else {
			log_info(fm9_log, "El cliente %d se ha conectado correctamente",
					socket);
		}
		break;
	case INSERTAR:
		linea = malloc(fm9.max_linea);
		log_info(fm9_log, "INSERTAR LINEA...");

		recv(socket, &pid, sizeof(int), MSG_WAITALL);
		log_info(fm9_log, "pid: %d", pid);

		recv(socket, linea, fm9.max_linea, MSG_WAITALL);
		log_info(fm9_log, "LINEA: %s", linea);

//		int memoria_requerida = *((int*) datos);
		datos += sizeof(int);
		char* file_name = malloc(strlen(datos));
		memcpy(file_name, datos, strlen(datos));
		break;
	case DESCARGAR:
		break;
	}
}

void coordinarCPU(int socket, void* datos, header_paquete* paquete) {
	switch (paquete->tipo_operacion) {
	case HANDSHAKE:
		log_info(fm9_log, "El cliente %d se ha conectado correctamente",socket);
		break;
	case ACTUALIZAR:
		break;
	case ELIMINAR:
		break;
	}
}

void iniciar_administracion_memoria() {
	inicializar_memoria();
	switch (fm9.modo) {
	case SEG:
		iniciar_segmentacion_pura(fm9.tamanio);
		break;
	case SPA:
		iniciar_segmentacion_paginada(fm9.tamanio);
		break;
	case TPI:
		iniciar_paginas_invertidas(fm9.tamanio);
		break;
	}
}

void liberar_recursos(int tipo_salida) {
	print_footer(FM9, fm9_log);
	destruir_archivo_log(fm9_log);
	terminar_exitosamente(tipo_salida);
}

void terminar_exitosamente(int ret_val) {
	if (socket_fm9 != 0)
		close(socket_fm9);
	exit(ret_val);
}
