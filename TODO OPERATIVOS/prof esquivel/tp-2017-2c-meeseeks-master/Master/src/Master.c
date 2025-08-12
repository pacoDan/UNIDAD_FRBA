#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <commons/config.h>

typedef struct Configuracion {
	char *configYamaIP;
	int configYamaPuerto;
} t_configuracion;

int imprimirConfiguracionPorPantalla(t_configuracion *configuracion){
	printf("Info del archivo config.cfg de Master:\n");
	printf("IP: %s\n", configuracion->configYamaIP);
	printf("Puerto: %d\n",configuracion->configYamaPuerto);

	return EXIT_SUCCESS;
}

int levantarConfiguracionDeArchivo(t_configuracion *configuracion, char *ubicacionArchivoDeConfiguracion){
	t_config* config;
	int numeroDeError = -10;

	config = config_create(ubicacionArchivoDeConfiguracion);

	if(config==NULL){
		puts("ERROR!!!: Archivo de configuracion no recuperado, funcion config_create devolvio null");
		config_destroy(config);
		return numeroDeError;
	}
	else{//archivo recuperado correctamente

		if(config_has_property(config, "YAMA_PUERTO")){
			configuracion->configYamaPuerto = config_get_int_value(config, "YAMA_PUERTO");
			puts("Puerto recuperado");
		}
		else{
			configuracion->configYamaPuerto = NULL;
			puts("ERROR!!!: Puerto no recuperado");
		}

		if(config_has_property(config, "YAMA_IP")){
			configuracion->configYamaIP = string_from_format("%s", config_get_string_value(config, "YAMA_IP"));
			puts("IP kernel recuperada");
		}
		else{
			configuracion->configYamaIP = NULL;
			puts("ERROR!!!: IP kernel no recuperada");
		}

		config_destroy(config);

		if(configuracion->configYamaIP != NULL && configuracion->configYamaPuerto != NULL){
			puts("Toda la informacion del archivo de configuracion fue recobrada exitosamente");
			return EXIT_SUCCESS;
		}
		else{
			puts("Al menos un valor no pudo ser recobrado");
			return numeroDeError;
		}
	}//fin archivo recuperado correctamente
	return EXIT_SUCCESS;
}

int obtenerDirectorioDeConfiguracion(char *ubicacionArchivoDeConfiguracion){
	printf("Ingrese la direccion del archivo de configuracion: ");
	scanf("%s", ubicacionArchivoDeConfiguracion);
	return EXIT_SUCCESS;
}


int main(){
	/*
	* getaddrinfo() = Obtiene los datos de la direccion de red y lo guarda en serverInfo.
	*/
	char puerto[100];
	int resultado;
	t_configuracion configuracion;
	char ubicacionArchivoDeConfiguracion[100]; //esta en "config.cfg"

	puts("Soy Master"); /* prints Soy Master */

	obtenerDirectorioDeConfiguracion(ubicacionArchivoDeConfiguracion);
	resultado=levantarConfiguracionDeArchivo(&configuracion, ubicacionArchivoDeConfiguracion);//busca la configuracion, en caso de error me de vuelve -10
	//resultado=levantarConfiguracionDeArchivo(&configuracion, "/home/utnso/workspace_2C/tp-2017-2c-meeseeks/Master");

	if(resultado == 0)
		imprimirConfiguracionPorPantalla(&configuracion);//muestra la configuracion x pantalla
	else{
		printf("Error al levantar la configuracion de Master\n");
		return resultado;
	}

	return EXIT_SUCCESS;
}
