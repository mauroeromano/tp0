#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	// ! creo un log_info para que muestre el mensaje "Hola! Soy un log"
	// printf("Hola! Soy un log"); muestra por consola, el log_info te muestra ademas por archivo y con la informacion y el historial
	log_info (logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// ! le paso los valores del archivo cliente.config a los char* ip, puerto y valor
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config

	// ! vuelvo a imprimir el mensaje
	log_info(logger, "IP: %s",ip);
	log_info(logger, "PUERTO: %s",puerto);
	log_info(logger, "VALOR: %s",valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);

	// ! ademas de la clave, quiero enviar el ip y el puerto
	enviar_mensaje(ip,conexion);
	enviar_mensaje(puerto,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	// ! el paquete lo envio desde la funcion paquete

	terminar_programa(conexion, logger, config);
}


// ! creo un logger con los parametros que me da el enunciado "tp0.log", -, true, LOG_LEVEL_INFO
t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0-LOGGER", true, LOG_LEVEL_INFO);

	// ! chequeo que en caso de error retorne un mensaje de error
	if(nuevo_logger == NULL){
		perror("Error en la creacion del logger");
		abort();
	}

	return nuevo_logger;
}

// ! creo el config con la ruta (path) que va al archivo client.config

t_config* iniciar_config(void)
{
	// t_config* nuevo_config = config_create("cliente.config");

	// ! hardcodeo el path para poder correrlo desde la consola:
	t_config* nuevo_config = config_create("/home/utnso/gitHubRepositores/tp0/client/cliente.config");
	

	// ! chequeo que en caso de error retorne un mensaje de error
	if(nuevo_config == NULL){
		perror("Error en la creacion del config");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	// ! hago un log info para imprimir lo que leyo el readline
	log_info(logger, "leido: %s",leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ! hago un bucle para leer hasta que reciba un EOF
	while((strcmp(leido, "")) != 0){
		free(leido);
		leido = readline("> ");
		log_info(logger, "leido: %s",leido);
	}
	
	// ¡No te olvides de liberar las lineas antes de regresar!
	// ! libero la memoria
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	// ! uso la misma logica que en la lectura y ademas agrego al paquete
	leido = readline("> ");
	while((strcmp(leido, "")) != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	// ! envio el paquete
	enviar_paquete(paquete, conexion);

	// ! elimino el paquete
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	// ! pongo la funcion para terminar el logger y le paso el logger a terminar
	log_destroy(logger);
	// ! pongo la funciona para terminar el config
	config_destroy(config);
	// ! libero la conexion
	liberar_conexion(conexion);
}
