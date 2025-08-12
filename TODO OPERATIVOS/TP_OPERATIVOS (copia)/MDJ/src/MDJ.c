/*
 ============================================================================
 Name        : MDJ.c
 Author      : Quantum
 Version     :
 Copyright   : Sistemas Operativos 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "MDJ.h"

pthread_t hilo_escucha_v2;
pthread_t hilo_consola_fifa_v2;

pthread_t hilo_inicio_aux;



int main(void) {

	pthread_create(&hilo_inicio_aux, NULL, cargarConfiguracionMDJ,NULL);
	pthread_join(hilo_inicio_aux, NULL);
	pthread_attr_destroy(&hilo_inicio_aux);

	pthread_create(&hilo_escucha_v2, NULL,escuchaMensajes,NULL);
//	pthread_create(&hilo_consola_fifa_v2, NULL, &consola_fifa, NULL);

//	pthread_join(&hilo_escucha_v2, NULL);
//	pthread_join(&hilo_consola_fifa_v2,NULL);

	pthread_attr_destroy(&hilo_escucha_v2);
//	pthread_attr_destroy(&hilo_consola_fifa_v2);

	consola_fifa();


bitarray_destroy(bitarray_);
	return 0;
}

void cargarConfiguracionMDJ(){

	cargar_configuracion_mdj("mdj.config");
	mostrar_configuracion_mdj();
	cargar_configuracion_metadata("Metadata.bin");
	mostrar_configuracion_metadata();
	cargar_configuracion_bitmap();

	setear_bloque_ocupado_en_posicion(62);
	setear_bloque_ocupado_en_posicion(61);
	setear_bloque_ocupado_en_posicion(5);


	mostrar_bitarray();
}

void consola_fifa(){
	puts("press \"exit\" para salir de consola ");
	loop{
			buffer_input_keyboard=readline("fifa@mdj=>  ");
			if(!strncmp(buffer_input_keyboard, "exit", 4)) break;
			ejecutar_linea_entrante(buffer_input_keyboard);
			free(buffer_input_keyboard);
	}
}
void  ejecutar_linea_entrante(char* buffer_entrante){
//	system(buffer_entrante);
	persistirContenido(buffer_entrante);
}
void persistirContenido(char * contenido){
	 bloqueActual_path=malloc(MAX_INPUT_BUFFER);
	do {
		getBloqueLibre_path();
		persistirAlBloque(bloqueActual_path,contenido);
	} while (quedaContenidoParaMapear(contenido));
//			setear_bloque_ocupado_en_posicion(bloqueActual_int);
	free(bloqueActual_path);
}


 int  espacioRestanteAlBloque(char* pathDelBloque){
	int resto =  metadata.tamanio_de_bloque-cantidadDeCaracteres_path(pathDelBloque);
//	if (resto<0) { // no deberia de pasar esto
//		setear_bloque_ocupado_en_posicion(bloqueActual_int);
//	}
	return resto;
}


void  persistirAlBloque(char* unBloquePath, char * contenido){
	char* recorte =NULL;
	recorte=recortarPrimerosCaracteres(contenido,espacioRestanteAlBloque(unBloquePath));
	printf("se va a mapear al bloque %s ,contenido: %s y el restante es %s \n",bloqueActual_path,recorte,contenido);
//	 sprintf(bloqueActual_path,"%d.bin",bloqueActual_int);

	FILE* bloqueActual =txt_open_for_append(unBloquePath);
	txt_write_in_file(bloqueActual,recorte);
	txt_close_file(bloqueActual);
	free(recorte);
}
bool estaLLenoElBloqueActual(){
	return cantidadDeCaracteres_path(bloqueActual_path)==metadata.tamanio_de_bloque;
}

bool terminoDeMapearContenido(){//en revision
	bool hayCaracteresParaMapear=strlen(buffer_input_keyboard)>0;
	return bitarray_test_bit(bitarray_,bloqueActual_int)&&hayCaracteresParaMapear;
}


//INTERFAZ MDJ
bool validarArchivo(char* pathDelArchivo){//ver si existe el archivo, OK, se puede borrar todos los printf() y puts(),era para probar
  	puts("---------validacion de archivo-------");
	int contador_bloques_aux=0;
  	int cantidadDeBloques=0,bytesOcupados=0;
  		t_config* aux=config_create(pathDelArchivo);
  		if (aux==NULL) {
  			perror("->validarArchivo() ,no existe el archivo o path incorrecto");
  			return false;
		}
  		bytesOcupados=config_get_int_value(aux,"TAMANIO");
  		cantidadDeBloques=(bytesOcupados/metadata.tamanio_de_bloque);
  		if((bytesOcupados%metadata.tamanio_de_bloque)!=0)cantidadDeBloques++;//esto es importante ,es como saber el numero de paginas ocupadasb
  		char** bloques_aux= config_get_array_value(aux,"BLOQUES");
  		printf("cantidad de bloques  : %d \n",cantidadDeBloques);
  		printf("metadata tamanio del bloque : %d \n",metadata.tamanio_de_bloque);
  		printf("metadata tamanio del bloque : %d \n",metadata.tamanio_de_bloque);
  		printf("bytes ocupados :  %d \n",config_get_int_value(aux,"TAMANIO"));
  		puts("inicio ciclo for ");
  		for (int var = 0; var < cantidadDeBloques; var++) {
  			printf(" bloque %s iesimo ocupado ?\n", (bloques_aux[var]));
  			if(testear_bloque_libre_en_posicion(atoi(bloques_aux[var])))contador_bloques_aux++;
  		}
  		puts("despues d ciclo for ");
			printf("contador: %d y cantidad:  %d \n", contador_bloques_aux,cantidadDeBloques);

  		free(bloques_aux);
  		config_destroy(aux);
  		puts("-------------fin validacion de archivo-------------------");
	return contador_bloques_aux==cantidadDeBloques;
}
void obtener_datos(char* pathDelArchivo,int offset, int size){
	if(validarArchivo(pathDelArchivo))fprintf(stderr,"->obtener_datos() no se puede validar path : %s",pathDelArchivo);
	else{

	}
}
void crearArchivo(char* pathDelArchivo,int cantidadDeBytesDelArchivo){//OK
	puts("---------crearArchivo()-------------------");
	int bloques_a_ocupar=((cantidadDeBytesDelArchivo)/metadata.tamanio_de_bloque);
	if((cantidadDeBytesDelArchivo%metadata.tamanio_de_bloque)!=0)bloques_a_ocupar++;
	if(getCantidadDeBloquesLibres()<bloques_a_ocupar || bloques_a_ocupar<0)perror("No se puede crearArchivo(), espacio o bloques insuficientes");
	else{
		crearBloques(bloques_a_ocupar);
		t_config *aux = config_create(pathDelArchivo);
		if(aux==NULL)perror("error en crearArchivo(), en config");
		printf("bloques a ocupar :%d \n", bloques_a_ocupar);
		printf("TAMANIO :%d \n", cantidadDeBytesDelArchivo);
		char* s2 = getBloquesLibres_string(bloques_a_ocupar);
		printf("BLOQUES= %s \n",s2);
		config_set_value(aux,"BLOQUES",s2);
		config_set_value(aux,"TAMANIO",intToString(cantidadDeBytesDelArchivo));//preguntar  el tercer parametro
		config_save_in_file(aux,pathDelArchivo);
		config_destroy(aux);
		free(s2);
		puts("fin crearArchivo()");
	}
	puts("--------------fin crearArchivo()-----------");
}
char* getBloquesLibres_string(int cantidadDeBloques){//OK,solo para crearArchivo ,da en formato "[1,2,3,54,56,6]"
//	char* bloques_string=(char*)malloc(cantidadDeBloques*sizeof(char)*3);
	char* aux=strdup("[");
	t_list *bloques_libres_aux=getBloquesLibres_list(cantidadDeBloques);
	aux=str_concat(aux,intToString((int)list_get(bloques_libres_aux,0)));
	if(cantidadDeBloques==1){
			aux=str_concat(aux,"]");
			list_destroy(bloques_libres_aux);
			return aux;
	}
	for(int var = 1; var < cantidadDeBloques; var++) {
			aux=str_concat(aux,",");
			aux=str_concat(aux,intToString((int)list_get(bloques_libres_aux,var)));
	}
		list_destroy(bloques_libres_aux);
		aux=str_concat(aux,"]");
	return aux;
}

t_list* getBloquesLibres_list(int cantidadDeBloques){
	t_list *lista=list_create();
	for(int p=0;p<cantidadDeBloques;p++ ){
		list_add(lista,getBloqueLibre_int());
	}
	return lista;
}
int getCantidadDeBloquesLibres(){//ok
	int aux,  contador=0;
	for(aux=0;aux<metadata.cantidad_bloques;aux++){
		if(testear_bloque_libre_en_posicion(aux)==0)contador++;
	}
	return contador;
}


void guardar_datos(char* pathDelArchivo,int offset,int size, char* buffer){
	if(validarArchivo(pathDelArchivo))fprintf(stderr,"error en validar path : %s",pathDelArchivo);
	else {

	}
}

void borrarArchivo(char* pathDelArchivo){//ok
	puts("----------------borrarArchivo()---------");
	int cantidadDeBloques=0,bytesOcupados=0;
	t_config* aux=config_create(pathDelArchivo);
	if(aux==NULL)perror("->borrarArchivo() , no existe el archivo o path incorrecto");
	bytesOcupados=config_get_int_value(aux,"TAMANIO");
	cantidadDeBloques=(bytesOcupados/metadata.tamanio_de_bloque)+1;
	printf(" cantidad de bloques a borrar: %d \n",cantidadDeBloques);
	char** bloques_aux= config_get_array_value(aux,"BLOQUES");
	for(int var=0;var<cantidadDeBloques;var++){
		setear_bloque_libre_en_posicion(atoi(bloques_aux[var]));
		remove(pathDelArchivo);
	}
	free(bloques_aux);
	config_destroy(aux);
	puts("-------------fin de borrarArchivo()----------");
}

//INTERFAZ MDJ







