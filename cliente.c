 /* 
# * Author: David Vela Tirado
# * Software Developer
# * email:  david.vela.tirado@gmail.com
# * 
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>


#define MAXBUFF 100 //longitud máxima mensajes replicar con ECO
#define PORTDEST 5003  //puerto del cliente

void error (char * mensaje)
{
	perror (mensaje);
	exit(1);
}


int main()		// ./cliente  
{

int ds; 				//descriptor del socket
struct sockaddr_in servidor, destino;  //info. servidor y cliente 
int long_serv, long_dest, n , m ;	//LONGITUDES...
int serv_puerto;
char men_rec[MAXBUFF];  	//buffer de recepcion mensajes
char men_env[MAXBUFF];		//mensaje para enviar
char men_rec2[MAXBUFF];		//mensaje para recibir nombre del servidor
char localhost[MAXBUFF]= "127.0.0.1";

//inicializo la estructura destino:
	bzero( (char*) & destino, sizeof (destino));   //inicializo a cero la estructura
	destino.sin_family = AF_INET; 			//familia	
	destino.sin_port = htons (PORTDEST); /*formato red */	
	destino.sin_addr.s_addr = inet_addr(localhost); 		// IP de localhost

	long_dest = sizeof(destino);

	
for(;;)
{	
	//1 abrir socket (canal)
	if( (ds = socket(AF_INET, SOCK_STREAM, 0) ) == -1)
	{
		error("socket");
		
	}

	//2 conectar con el servidor
	if( connect(ds, (struct sockaddr *) &destino, long_dest) == -1)
	{
		error("connect");
		
	}
		
	//REALIZO LA LECTURA DE LOS DATOS DEL TECLADO:
		int i;
		fflush(stdin);
		
		for (i=0; i<MAXBUFF; i++)
		{	men_env[i]=0;
			men_rec[i]=0;
			men_rec2[i]=0;
		}		
			
	printf("\n ESCRIBIR CADENA: ");		
	if ( (fgets(men_env, MAXBUFF, stdin) == NULL) && (n=strlen(men_env)==0) ) 
		//COMPRUEBO: si es esta vacio me salgo
	{
		error("fgets");
				
	}
		
		/*int longfget = strlen(men_env);
		printf("\n el mensaje leido es %s de %d caracteres\n", men_env, longfget);
		men_env[longfget - 1]=0;
		 longfget = strlen(men_env);
		printf("\n el mensaje modificado es %s de %d caracteres\n", men_env, longfget);
		*/
	
//si me cargo el caracter fin de fichero al enviarme el servidor la cadena modificada
//la recibo junto al nombre del servidor...........

	if( (n=strlen(men_env)) == 1)
	{
		printf("\nCadena vacia.\n ADIOS.\n");
		break;
	}


	//3envio la cadena al servidor:
			
		
		m= write (ds, men_env, n);
		if(m == -1)
		{
			error("write");
					
		}

		if(m != n)
		{
			printf("\nse han perdido caracteres\n");
					
		}
			
	printf("\nLe he enviado esta cadena al servidor: %s\n",men_env);
		
		
	//4recibir datagrama:
		n= read(ds, men_rec, MAXBUFF);
		if(n == -1)
		{ 
			error("read1");
				
		}
		printf("\nHe recibido: %s ", men_rec);

		n= read(ds, men_rec2, MAXBUFF);
		if(n == -1)
		{ 
			error("read2");
				
		}
		printf("\n He recibido: %s ", men_rec2);
			
	int longitudreci=strlen(men_rec);
	//VISUALIZO LA CADENA RECIBIDA.
			printf("\n NOMBRE DEL SERVIDOR: %s",men_rec2 );
			printf("\n CADENA RECIBIDA: %s de longitud: %d", men_rec,longitudreci);
			printf("\n\n\n");
		
	

	close(ds); //5 cerrar el socket
	}		
	
}
		
		
