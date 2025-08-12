#ifndef CPU_H_
#define CPU_H_


#include <commons/string.h> //Commons string
#include <qcommons/console.h>
#include <qcommons/socket.h> //Libreria Socket Cliente
#include <qcommons/protocolos.h>
#include <signal.h>
#include <stdbool.h>

#include "config/config.h"//Llamadas para leer y mostrar el archivo config
#include "parser/parser.h"//parser del Lenguaje EscripTorio



#define FILE_CONFIG_CPU "cpu.config"
#define EXTENSION_FILE_LOG_CPU ".log"
#define CPU_NAME "CPU"

int socket_safa, socket_dam, socket_fm9;
struct_instruccion instruccion;
dtb_struct dtb_ejecutado;
//INFO: dtb_ejecutado lo voy a actualizar cada vez que ejecuto un dtb



//Funciones
void conectarse_con_safa();
void conectarse_con_dam();
void conectarse_con_fm9();
int inicializar(char* nombre_archivo_log);
char* crear_nombre_file_log();
void liberar_recursos(int tipo_salida);
void exit_gracefully();

void ejecutar_instruccion(struct_instruccion instruccion);
void liberar_instruccion(struct_instruccion instruccion);


//////EJECUCION DE OPERACIONES//////
unsigned escriptorio_abrir(char** parametros);
unsigned escriptorio_concentrar(char** parametros);
unsigned escriptorio_asignar(char** parametros);
unsigned escriptorio_wait(char** parametros);
unsigned escriptorio_signal(char** parametros);
unsigned escriptorio_flush(char** parametros);
unsigned escriptorio_close(char** parametros);
unsigned escriptorio_crear(char** parametros);
unsigned escriptorio_borrar(char** parametros);
unsigned escriptorio_comentario(char** parametros);
///////////////////////////////////

bool se_encuentra_archivo_en_gdt(char* path);
void DAM_abrir(uint8_t id_dtb,char * path);
void desalojar_dtb();
void SAFA_avisar_espera_de_carga(char *path);

void FM9_actualizar(char *path,char *linea,char *datos);
void abortar_gdt(int8_t id_dtb);

void DAM_flush(char *path);
void SAFA_espera_flush(uint8_t id_gdt);


void FM9_solicitar_liberar_memoria(char *path);
void SAFA_borrar_referencia(char *path);


void DAM_solicitar_crear_archivo(char *path,char *lineas);
void DAM_solicitar_borrar(char* path);


#endif /* CPU_H_ */
