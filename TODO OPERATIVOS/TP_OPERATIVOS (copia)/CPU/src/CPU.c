#include "CPU.h"

void signal_catch(int signal);
int validar_parametros_consola(int cant_parametros);

int main(int argc, char *argv[]) {
	if (validar_parametros_consola(argc) != 0) {
		return EXIT_FAILURE;
	}

	if (inicializar(argv[1]) < 0) {
		liberar_recursos(EXIT_FAILURE);
		return -1;
	}

	imprimir_config();

	//conectarse_con_safa();

	conectarse_con_dam();
	//conectarse_con_fm9();

	DAM_abrir(5,"queondawacho/todopiola/");


	/**
	 * ####Lectura de Escriptorio
	 *
	FILE * archivo;
	char * linea = NULL;
	size_t longitud = 0;
	ssize_t leido;
	archivo = fopen(argv[2], "r");
	if (archivo == NULL) {
		log_error(cpu_log, "No se puedo abrir el Escriptorio.");
		liberar_recursos(EXIT_FAILURE);
	}
	while ((leido = getline(&linea, &longitud, archivo)) != -1) {
		//log_info(cpu_log, "%s", linea);
		instruccion = parsear_linea(linea);
		ejecutar_instruccion(instruccion);
	}

	fclose(archivo);
	if (linea)
		free(linea);
*/


	//liberar_recursos(EXIT_SUCCESS);
}

void conectarse_con_fm9(){
	obtener_socket_cliente(&socket_fm9,cpu.ip_fm9,cpu.puerto_fm9);
	//ejecutar_handshake(socket_fm9,"DAM",DAM,dam_log);

	header_paquete* paquete = malloc(sizeof(header_paquete));
	paquete->tipo_instancia = CPU;
	paquete->tipo_operacion = HANDSHAKE;

	log_info(cpu_log,"HANDSHAKE con FM9");
	send(socket_fm9,paquete,sizeof(header_paquete),0);
	//recv(socket_fm9,&MAX_LINEA,sizeof(int),MSG_WAITALL);

	//free(paquete);

	//log_info(cpu_log,"TAM LINEA: %d", MAX_LINEA);
}


void  conectarse_con_dam(){
	log_info(cpu_log, "Conectandome a DAM.");
	obtener_socket_cliente(&socket_dam,cpu.ip_dam,cpu.puerto_dam);



}

void  conectarse_con_safa(){
	log_info(cpu_log, "Conectandome a SAFA.");
	obtener_socket_cliente(&socket_safa,cpu.ip_safa,cpu.puerto_safa);

}




void ejecutar_instruccion(struct_instruccion instruccion){
	switch (instruccion.nombre_instruccion) {
		case ESCRIPTORIO_ABRIR:
			escriptorio_abrir(instruccion.parametros);
			break;
		case ESCRIPTORIO_CONCENTRAR:
			escriptorio_concentrar(instruccion.parametros);
			break;
		case ESCRIPTORIO_ASIGNAR:
			escriptorio_asignar(instruccion.parametros);
			break;
		case ESCRIPTORIO_WAIT:
			escriptorio_wait(instruccion.parametros);
			break;
		case ESCRIPTORIO_SIGNAL:
			escriptorio_signal(instruccion.parametros);
			break;
		case ESCRIPTORIO_FLUSH:
			escriptorio_flush(instruccion.parametros);
			break;
		case ESCRIPTORIO_CLOSE:
			escriptorio_close(instruccion.parametros);
			break;
		case ESCRIPTORIO_CREAR:
			escriptorio_crear(instruccion.parametros);
			break;
		case ESCRIPTORIO_BORRAR:
			escriptorio_borrar(instruccion.parametros);
			break;
		case ESCRIPTORIO_COMENTARIO:
			escriptorio_comentario(instruccion.parametros);
			break;
		default:
			break;
	}
	(dtb_ejecutado.quantum)-=1;
	//TODO: contabilizar el quantum ejecutado
	//hacer confirmacion(send) de ejecucion de 1 unidad de quantum

	liberar_instruccion(instruccion);
}

/*
 * TODO: en estas funciones va a estar la logica del anexo 1
 * */
unsigned escriptorio_abrir(char** parametros){

	char * path = parametros[0];
	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		return 0;
	}else{
		DAM_abrir(dtb_ejecutado.id_dtb,path);
		desalojar_dtb();
		SAFA_avisar_espera_de_carga(path);
	}

	return 0;
}

void desalojar_dtb(){
	/*TODO: borrar dtb
	*indicar a SAFA que CPU esta esperando que DAM cargue en FM9 el archivo del path
	*/

}

void SAFA_avisar_espera_de_carga(char *path){
	//TODO: SAFA_avisar_espera_de_carga
}

void DAM_abrir(uint8_t id_dtb,char * path){
	//TODO: enviar solicitud a diego para traer path desde mdj
	operacion_archivo operacion_abrir;
	operacion_abrir.pid = id_dtb;
	operacion_abrir.ruta_archivo = strdup(path);
	int tamanio;
	void * buffer = serializar_operacion_archivo(&operacion_abrir,&tamanio);
	header_paquete* paquete = malloc(sizeof(header_paquete));
	paquete->tipo_instancia = CPU;
	paquete->tipo_operacion = ABRIR;
	paquete->tamanio_mensaje = tamanio;

	log_info(cpu_log,"OPERACION ABRIR DAM");
	send(socket_dam,paquete,sizeof(header_paquete),0);
	send(socket_dam,buffer,tamanio,0);

}

bool se_encuentra_archivo_en_gdt(char *path){
	bool coincide_el_path(char * direccion){
		return string_equals_ignore_case(direccion, path);
	}
	return list_find(dtb_ejecutado.direcciones, (void*)coincide_el_path);
}



unsigned escriptorio_concentrar(char** parametros){
	sleep(cpu.retardo);
	return 0;
}



unsigned escriptorio_asignar(char** parametros){
	char * path = parametros[0];
	char * linea = parametros[1];
	char * datos = parametros[2];

	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		FM9_actualizar(path,linea,datos);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}


	return 0;
}

void FM9_actualizar(char *path,char *linea,char *datos){
	//TODO hacer un send de los datos a actualizar a FM9
	//hacer receive del resultado de la operacion desde FM9
	//20002: Fallo de segmento/memoria.
	//20003: Espacio insuficiente en FM9.
	//hacer el send a SAFA
}

void abortar_gdt(int8_t id_dtb){
	//TODO: enviar a safa el id del DTB para que
	//se aborte de forma gratuita y segura
}

unsigned escriptorio_wait(char** parametros){return 0;}
unsigned escriptorio_signal(char** parametros){return 0;}

unsigned escriptorio_flush(char** parametros){
	char * path = parametros[0];

	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		DAM_flush(path);
		SAFA_espera_flush(dtb_ejecutado.id_dtb);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}
	return 0;
}

void DAM_flush(char *path){
	//TODO: DAM_flush
}

void SAFA_espera_flush(uint8_t id_gdt){
	//TODO: SAFA_espera_flush
}

unsigned escriptorio_close(char** parametros){
	char * path = parametros[0];
	if(se_encuentra_archivo_en_gdt(path)){
		printf("El archivo se encuentra abierto %s",path);
		FM9_solicitar_liberar_memoria(path);
		SAFA_borrar_referencia(path);
		return 0;
	}else{
		//TODO: enviar a safa 	//20001: El archivo no se encuentra abierto.
		abortar_gdt(dtb_ejecutado.id_dtb);
	}


	return 0;
}

void FM9_solicitar_liberar_memoria(char *path){
	//TODO: FM9_solicitar_liberar_memoria
}

void SAFA_borrar_referencia(char *path){
	//TODO: SAFA_borrar_referencia
}

unsigned escriptorio_crear(char** parametros){
	char * path = parametros[0];
	char * lineas = parametros[1];

	DAM_solicitar_crear_archivo(path,lineas);
	desalojar_dtb();

	return 0;
}

void DAM_solicitar_crear_archivo(char *path,char *lineas){
	//TODO:DAM_solicitar_crear_archivo
}

unsigned escriptorio_borrar(char** parametros){
	char * path = parametros[0];

	DAM_solicitar_borrar(path);
	desalojar_dtb();
	return 0;
}

void DAM_solicitar_borrar(char* path){
	//TODO: DAM_solicitar_borrar
}

unsigned escriptorio_comentario(char** parametros){return 0;}//no hace nada

void liberar_instruccion(struct_instruccion instruccion){
	string_iterate_lines(instruccion.parametros,(void *)free);
}


int validar_parametros_consola(int cant_parametros) {
	if (cant_parametros < 2) {
		printf("Ingrese un nombre para el archivo de log!\n");
		return -1;
	}

	if (cant_parametros < 3) {
		printf("Ingrese una ruta a un script Escriptorio!\n");
		return -1;
	}

	return 0;
}

int inicializar(char* nombre_archivo_log) {
	char* name = crear_nombre_file_log(nombre_archivo_log);
	if (create_log(name) == EXIT_FAILURE)
		exit_gracefully(EXIT_FAILURE);

	print_header(CPU_NAME, cpu_log);
	free(name);

	if (cargar_archivo_config(FILE_CONFIG_CPU) < 0) {
		return -1;
	}

	return 0;
}

char* crear_nombre_file_log(char* nombre) {
	char* log_file_name = malloc(10);
	strcpy(log_file_name, nombre);
	strcat(log_file_name, EXTENSION_FILE_LOG_CPU);
	return log_file_name;
}

void liberar_recursos(int tipo_salida) {
	liberar_recursos_configuracion();
	print_footer(CPU_NAME, cpu_log);
	log_destroy(cpu_log);
	exit_gracefully(tipo_salida);
}

void signal_catch(int signal) {
	printf("\nTratando seniales, senial: %d\n", signal);
	switch (signal) {
	case SIGINT:
	case SIGKILL:
	case SIGSTOP:
	case SIGTSTP:
		liberar_recursos(EXIT_SUCCESS);
	}
}

void exit_gracefully(int ret_val) {
	close(socket_safa);
	close(socket_dam);
	close(socket_fm9);
	exit(ret_val);
}
