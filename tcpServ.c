#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#define PUERTO 8600
#define PAQUETE 1400

double timeval_diff(struct timeval *a, struct timeval *b)
{
    return (double)(a->tv_sec + (double)a->tv_usec/1000000.0)-(double)(b->tv_sec + (double)b->tv_usec/1000000.0);
}

int main()
{
    struct timeval ini, fin;
    int servidor, yes=1,cs,tam=1024*32;
    struct sockaddr_in servaddr;
    struct sockaddr_in remote_addr;
    char buffer[PAQUETE];
    if (( servidor= socket (AF_INET, SOCK_STREAM, 0)) <0)
    {
		perror("Problema creando el Socket.");
		exit(1);
	}
    if( setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0 )
    {
		close(servidor);
        perror("setsockopt");
		exit(1);
    }
    if(setsockopt(servidor, SOL_SOCKET, SO_SNDBUF, &tam, sizeof(int)) < 0)
	{
		close(servidor);
        perror("setsockopt");
		exit(1);
	}
    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PUERTO);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(servidor,(struct sockaddr*)&servaddr,sizeof(servaddr))== -1 )
    {
        perror("bind");
		close(servidor);
        exit(1);
    }
    if(listen(servidor,10) == -1)   
    {
        perror("listen");
		close(servidor);
        exit(EXIT_FAILURE);
    }
    tam = sizeof(remote_addr);	
    if((cs=accept(servidor,(struct sockaddr*)&remote_addr ,&tam))<0)
	{
        perror("accept");
        exit(1);
    } 
    tam=1024*32;
    if(setsockopt(cs, SOL_SOCKET, SO_SNDBUF, &tam, sizeof(int)) < 0)
	{
		close(servidor);
        perror("setsockopt");
		exit(1);
	}
    int total=0;
    printf("Conexion aceptada, inicia transferencia\n");
    gettimeofday(&ini,NULL);
    for(int i=0; i<10000; i++)
    {
        memset(buffer,0,PAQUETE);
        if( (tam=recv(cs,buffer,PAQUETE,0) ) <= 0) //error o conexion cerrada
        {   
            if(tam<0)
            {
                perror("recv cliente");          
            }
        }
        else
        {
            total+=tam;
        }
    }
    gettimeofday(&fin,NULL);
    printf("Recibidos %d bytes por TCP en %.g milisegundos\n",total,1000*timeval_diff(&fin,&ini));    
    return 0;
}
