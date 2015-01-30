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
#define MAXCONECT 5

void error (char * mensaje)
{
	perror (mensaje);
	exit(1);
}

int main( int argc,char *argv[])		// ./servidor "puerto" "nombre del servidor"  
{
	if( argc!=3)
	{
		printf("\nEl numero de argumentos es incorrecto.\n");
		exit(0);
	}

int ds; 				//descriptor del socket
struct sockaddr_in servidor, cliente;  //info. servidor y cliente 
int long_serv, long_cli, n , m ,p,newds;	//LONGITUDES...
int serv_puerto;
char men_rec[MAXBUFF];  	//buffer de recepcion mensajes
char men_env[MAXBUFF];		//mensaje para enviar
serv_puerto=atoi(argv[1]);		//convierto de char a int	

//inicializo la estructura del servidor
	long_cli=sizeof(cliente);
	bzero( (char*) &cliente, long_cli); 	//inicializo a cero la estructura	
	bzero( (char*) & servidor, sizeof (servidor));   //inicializo a cero la estructura
	
	servidor.sin_family = AF_INET; 			//familia	
	servidor.sin_addr.s_addr = INADDR_ANY; 		//cualquier IP
	servidor.sin_port = htons (serv_puerto); /*formato red */
	
	long_serv = sizeof(servidor);


//1 abrir socket (canal)

	if( (ds = socket(AF_INET, SOCK_STREAM, 0) ) == -1)
	{
		error("socket");
		
	}

//2 asociar el canal y el puerto

	if( bind(ds, (struct sockaddr *) &servidor, long_serv) == -1)
	{
		error("bind");
		
	}
	
	
//3 ESCUCHA
	printf("\nAhora me pongo a escuchar peticiones de conexion.");
	int listen_dev;
	  listen_dev=listen(ds,MAXCONECT);
		if(listen_dev==-1)
		{
			error("listen");
		}
	
	printf("\n LISTEN OK ");
		
	
//PROCESO
for(;;)
{
	
	//ACEPTAR LA CONEXION:
	if( (newds=accept(ds,(struct sockaddr *) &cliente, &long_cli)) == -1)
	{
		error("accept");
	}

	printf("\nel nuevo socket asignado para el circuito virtual es %d ",newds);
		
		
		int is;
		fflush(stdin);
		for (is=0; is<MAXBUFF; is++)
		{	men_env[is]=0;
			
			men_rec[is]=0;
		}	

	//recibo la cadena:
		n= read(newds, men_rec, MAXBUFF);
		if(n == -1)
		{ 
			error("read");
				
		}
	printf("\n Mensaje recibido: %s \n Longitud: %d .",men_rec,strlen(men_rec));
	
	//REALIZO EL SERVICIO:
		//invertir el mensaje:
			int i;	 	//indice de la cadena inversa
			int d=n-1;	//indice de la cadena original
				
			for( i=0; i<n; i++, d--)
			{
				men_env[i] = men_rec[d];
			}
		
	//4envio el mensaje al cliente:	
			m= write (newds, men_env, n);
			if(m == -1)
			{
				error("write 1");
					
			}
			if(m != n)
			{
				printf("\nse han perdido caracteres\n");
					
	
			}
	
	printf("\n Mensaje enviado al cliente: %s \n",men_env);
		

	//envio el nombre del servidor pasado por la linea de ordenes

			p=strlen(argv[2]);
			n= write (newds, argv[2],p);
			if(p == -1)
			{
				error("write 2");
					
			}
			if(n != p)
			{
				printf("\nse han perdido caracteres en el nombre del servidor\n");
					
			}
			

	printf("\n Mensaje enviado al cliente: %s \n",argv[2]);

	close(newds);	//5 cerrar el socket

}		
		
close(ds); //cierro el socket de escucha.

}
		
		





