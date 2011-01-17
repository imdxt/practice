#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT     20000
#define BUFFER_SIZE     256

char buffer[BUFFER_SIZE];

int main(int argc, const char *argv[])
{
    char c;
    int server_sock,client_sock;
    int i,len,n;
    socklen_t client_len;
    struct sockaddr_in server,client;
    struct in_addr in;

    if((server_sock = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket error");
        exit(1);
    }
    else
        printf("create socket success!!\n");   
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = htons(INADDR_ANY);
    
    if(bind(server_sock,(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        perror("bind error");
        exit(1);
    }
    else
        printf("bind addr success !!\n");

    if(listen(server_sock,5)<0)
    {
        perror("listen error");
        exit(1);
    }
    else
        printf("listen success!!\n");
    
    while(1)
    {
        client_len = sizeof(client);
        if((client_sock = accept(server_sock,(struct sockaddr *)&client,&client_len))<0)
        {
            close(server_sock);
            perror("accept error");
            exit(1);
        }
        else
        {
            printf("accept client success!!\n");
            in.s_addr = client.sin_addr.s_addr;
            printf("client ip   :%s\n",inet_ntoa(in));
            printf("client port :%d\n",ntohs(client.sin_port));
        }

        while((len = recv(client_sock,buffer,BUFFER_SIZE,0))>0)
        {
            if(buffer[0] == 'Q') break;
            for(i=0; i<len;i++)
            {
                c = buffer[i];
                buffer[i] = toupper(c);
            }
            n = send(client_sock,buffer,len,0);
        }
        close(client_sock);

        printf("Client close the connect, wait for new connect\n");
        putchar('\n');
    }
    close(server_sock);
    exit(0);
}
