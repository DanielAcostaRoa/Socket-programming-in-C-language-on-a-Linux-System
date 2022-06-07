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

#define PUERTO 8500
#define PAQUETE 1440

double timeval_diff(struct timeval *a, struct timeval *b)
{
    return (double)(a->tv_sec + (double)a->tv_usec/1000000.0)-(double)(b->tv_sec + (double)b->tv_usec/1000000.0);
}

int main()
{
    struct timeval ini, fin;
    int servidor, yes=1,cs,tam=5000*1024, rt;
    struct sockaddr_in servaddr;
    struct sockaddr_in remote_addr;
    char buffer[PAQUETE];
    if (( servidor= socket (AF_INET, SOCK_DGRAM, 0)) <0)
    {
		perror("Problema creando el Socket.");
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
    int total=0;
    gettimeofday(&ini,NULL);
    for(int i=0; i<1000; i++)
    {
        memset(buffer,0,PAQUETE);
        if( (tam=recvfrom(servidor,buffer,PAQUETE,0, (struct sockaddr*)&remote_addr,&rt ) ) <= 0) //error o conexion cerrada
        { 
            if(tam<0)
            {
                perror("recv cliente");          
            }
        }
        else
        {
           if(i==0) gettimeofday(&ini,NULL);
           total+=tam;    
        }
    }
    gettimeofday(&fin,NULL);
    printf("Recibidos %d bytes por UDP en %.g milisegundos\n",total,1000*timeval_diff(&fin,&ini));     
    return 0;
}
