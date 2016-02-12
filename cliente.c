/***************************************************************************************
*										       *
*			NET DOWN - qualix DEVELOPS - www.qualix.com.ar                     *
*                                    Cliente                                           *
****************************************************************************************
* Descripcion: 	Aplicacion cliente-servidor para ejecutar comandos por el cliente de   *
*		atraves del servidor, de manera remota.                                *
*                                                                                      *
****************************************************************************************
* Autor: Fernando Abad - qualix- fabad@qualix.com.ar  *  Version:	0.1                    *
****************************************************************************************
* fecha: 26.11.2010						       *
***************************************************************************************/



/************************************* Headers *****************************************/

#include <stdio.h>          
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <netdb.h>  					/* netbd.h es necesitada por la estructura hostent */

/************************************* Define's *****************************************/

#define PORT 		3550         			/* El Puerto Abierto del nodo remoto 	*/
#define MAXDATASIZE 	100	  			/* El número máximo de datos en bytes 	*/

/*********************************** Funcion Principal **********************************/

int main(int argc, char *argv[]){
   
   int fd, numbytes;          				/* ficheros descriptores */
   char buf[MAXDATASIZE];  				/* en donde es almacenará el texto recibido */

   struct hostent *he;         				/* estructura que recibirá información sobre el nodo remoto */
   struct sockaddr_in server;  				/* información sobre la dirección del servidor */

   if (argc !=2) { 					/* el programa sólo necesitará un argumento, (la IP) */ 
      printf("%s%s","Uso: <Dirección IP>\n",argv[0]);	/* si no se ingresa el argumento sale de la aplicacion */
      return EXIT_FAILURE;				
      }

   if ((he=gethostbyname(argv[1]))==NULL){       	/* llamada a gethostbyname() */
      printf("gethostbyname() error\n");		/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
      }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  	/* llamada a socket(), se crea el socket */
      printf("socket() error\n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
      }

   server.sin_family = AF_INET;				/* inicializacion del socket */
   server.sin_port = htons(PORT); 			/* htons() es necesaria nuevamente */
   server.sin_addr = *((struct in_addr *)he->h_addr);   /* he->h_addr pasa la información de *he a h_addr" */
   bzero(&(server.sin_zero),8);				/* escribimos ceros en el reto de la estructura */

   if(connect(fd, (struct sockaddr *)&server,		
      sizeof(struct sockaddr))==-1){ 			/* llamada a connect(), se crea la conexion en una direccion */
      printf("connect() error\n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
      }

   if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  	/* llamada a recv(), se crea la conexion en la otra direccion */
      printf("Error en recv() \n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
      }

   buf[numbytes]='\0';					/* se inicializa el buffer */
   printf("%s\n%s","Mensaje del Servidor: ",buf); 	/* muestra el mensaje del servidor */
   close(fd);   					/* cerramos la conexion */

   return EXIT_SUCCESS;					/* salimos de la aplicacion */
}
