#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "hacking.h"

#define PORT 7890

int main(int argc, char *argv[]){
    int sockfd,remote_sockfd,yes=1;
    struct sockaddr_in local,remote;
    
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        fatal("while creating the socket");
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
        fatal("while setting socket option");
    
    
    local.sin_addr.s_addr = 0;
    local.sin_port = htons(PORT);
    local.sin_family = AF_INET;
    memset(local.sin_zero,0,8);

    if(bind(sockfd,(struct sockaddr*)&local,sizeof(struct sockaddr)) == -1)
        fatal("while binding the socket");
    
    if(listen(sockfd,5) == -1)
        fatal("while listening the socket");
    
    while(1){
        socklen_t sin_size = sizeof(struct sockaddr);
        if((remote_sockfd = accept(sockfd,(struct sockaddr*)&remote,&sin_size)) == -1)
            fatal("while accepting the connection");
        printf("Accepted request from: %s:%d\n",inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));

        /*REMOTE*/
        send(remote_sockfd,"Hello world!\r\n",14,0);

        char *recieve = malloc(1024);
        int recv_size = recv(remote_sockfd,recieve,1024,0);

        while(recv_size > 0){
            printf("Recieved: %d bytes\n",recv_size);
            dump(recieve,recv_size);
            printf("\n");
            recv_size = recv(remote_sockfd,recieve,1024,0);
        }
        close(remote_sockfd);
        free(recieve);
        break;
    }
    return 0;
}