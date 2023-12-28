#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include "hacking-network.h"
#include "hacking.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <hostname>\n",argv[0]);
        exit(-1);
    }

    int sockfd;
    struct sockaddr_in remote;
    struct hostent *host_data;

    
    /*GET Hostname*/
    host_data = gethostbyname(argv[1]);
    if(host_data == NULL){
        fatal("cannot get host IP");
    }

    if((sockfd = socket(PF_INET,SOCK_STREAM,0)) == -1)
        fatal("while creating the socket");

    /*Create socket addres structure*/
    remote.sin_addr = *((struct in_addr*) host_data->h_addr_list[0]);
    remote.sin_port = htons(80);
    remote.sin_family = AF_INET;
    memset(remote.sin_zero,0,8);

    if(connect(sockfd,(struct sockaddr*)&remote,sizeof(struct sockaddr_in)) == -1)
        fatal("while connecting to the remote");
    
    send_string(sockfd,"HEAD / HTTP/1.0\r\n\r\n");

    unsigned char* buffer = (unsigned char*)malloc(1024);
    while(recv_line(sockfd,buffer)){
        if(strncasecmp(buffer,"Server:",7) == 0){
            printf("Webserver for %s : %s\n",inet_ntoa(remote.sin_addr) ,buffer+8);
            return 0;
        }

    }
    printf("Server not found\n");
    exit(1);
}
