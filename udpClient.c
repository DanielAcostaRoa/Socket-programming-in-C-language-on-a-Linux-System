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

#define PUERTO 8500
#define PAQUETE 1440



int main()
{
    int s, yes=1,c;
    struct sockaddr_in servaddr;
    char buffer[PAQUETE];
    if (( s= socket (AF_INET, SOCK_DGRAM, 0)) <0)
    {
		perror("Problema creando el Socket.");
		exit(1);
	}
    if( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0 )
    {
		close(s);
        perror("setsockopt");
		exit(1);
    }   
    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8500);
    if ((c=connect(s, (struct sockaddr*) &servaddr, sizeof(servaddr)))<0) 
	{
		if(errno!=EINPROGRESS)
		{	
			perror("connect");
			close(s);
			exit(3);
		}
		else
		{
			printf("Conexión en progreso\n");
		}
	}
	int h, enviados=0;
	for(int i=0; i<1000; i++)
	{
		memset(buffer,0,PAQUETE);
		if((h=send(s,buffer,sizeof(buffer),0)) <0 )
		{
			perror("send\n");
		}
		else
		{
			enviados+=h;
		}
	}
	printf("%d paquetes enviados\n",enviados);
	close(s);
    return 0;
}
