/*
 * mdj.h
 *
 *  Created on: 9 sep. 2018
 *      Author: utnso
 */
#ifndef MDJ_H_
#define MDJ_H_
#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h> //Commons config archivo
#include <commons/string.h> //Commons string

#include <commons/log.h>
#include <stdbool.h>
#include <unistd.h>


#include <stdarg.h>
#include <pthread.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "multiplexacion/multiplexacion.h"
#include "util/util.h"
#include "file_system/metadata.h"
#include "file_system/bitmap.h"

//----------------------------
char* buffer_input_keyboard;//string de entrada  de teclado;
//----------------------------
Socket mdj_socket; //socket MDJ

//begin auxiliares
void cargarConfiguracionMDJ();
//end auxiliares


void consola_fifa();
void  ejecutar_linea_entrante(char* buffer_entrante);
void persistirContenido(char * contenido);
 int  espacioRestanteAlBloque(char* pathDelBloque);
void  persistirAlBloque(char* unBloquePath, char * contenido);
bool estaLLenoElBloqueActual();
bool terminoDeMapearContenido();//en revision


//  begin INTERFAZ MDJ
bool validarArchivo(char* pathDelArchivo);//ver si existe el archivo, OK, se puede borrar todos los printf() y puts(),era para probar

void obtener_datos(char* pathDelArchivo,int offset, int size);
void crearArchivo(char* pathDelArchivo,int cantidadDeBytesDelArchivo);//OK
char* getBloquesLibres_string(int cantidadDeBloques);//OK,solo para crearArchivo ,da en formato "[1,2,3,54,56,6]"

t_list* getBloquesLibres_list(int cantidadDeBloques);
int getCantidadDeBloquesLibres();//ok

void guardar_datos(char* pathDelArchivo,int offset,int size, char* buffer);
void borrarArchivo(char* pathDelArchivo);//ok
//  end INTERFAZ MDJ



#endif /* MDJ_H_ */
