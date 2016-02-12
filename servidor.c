/***************************************************************************************
*										       *
*			NET DOWN - qualix DEVELOPS - www.qualix.com.ar                     *
*                                      Servidor                                        *
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
#include <netdb.h>  


/************************************* Define's *****************************************/

#define PORT 		3550				/* El puerto que será abierto */
#define BACKLOG 	2 				/* El número de conexiones permitidas */

/*********************************** Funcion Principal **********************************/

int main(void){

   int fd, fd2; 					/* los ficheros descriptores */
   int sin_size;
   struct sockaddr_in server; 				/* para la información de la dirección del servidor */
   struct sockaddr_in client; 				/* para la información de la dirección del cliente */
  
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {   /* llamada a socket(), se crea el socket */
      printf("error en socket...\n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      exit(-1);
   }

   server.sin_family = AF_INET;         
   server.sin_port = htons(PORT); 			/* htons() de la sección conversiones */
   server.sin_addr.s_addr = INADDR_ANY; 		/* INADDR_ANY coloca la dirección IP automáticamente */
   bzero(&(server.sin_zero),8);   			/* escribimos ceros en el reto de la estructura */

   
   if(bind(fd,(struct sockaddr*)&server,
           sizeof(struct sockaddr))==-1) {		/* la llamada a bind() */
      printf("error en bind... \n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
   	}     

   if(listen(fd,BACKLOG) == -1) {  			/* llamada a listen() */
      printf("error en listen... \n");			/* si no se puede llamar a la funcion sale de la aplicacion */
      return EXIT_FAILURE;
   	}

   while(1) {						/* lazo infinito para que el servidor quede online */
      sin_size=sizeof(struct sockaddr_in);		/* se setea el tamaño del socket */
      if ((fd2 = accept(fd,(struct sockaddr *)&client,	
                        &sin_size))==-1) {		/* la llamada a accept() */
         printf("error en accept... \n");			/* si no se puede llamar a la funcion sale de la aplicacion */
         return EXIT_FAILURE;
      	}

      printf("%s\n%c","Se obtuvo una conexión desde ",
              inet_ntoa(client.sin_addr) ); 		/* que mostrará la IP del cliente */
      send(fd2,"El servidor esta apagandose...\n",32,0); 	/* que enviará el mensaje de bienvenida al cliente */
      system("ls -la");					/* ejecuta el comando en el servidor */

      close(fd2); 					/* cierra la conexion */
   }
return EXIT_SUCCESS;					/* salimos de la aplicacion */

}

