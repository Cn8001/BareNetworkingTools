#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"hacking.h"

int main(void){
    int recv_len,sockfd;
    unsigned char buffer[8000];
    sockfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
    for(int i=0;i<100;i++){
        recv_len = recv(sockfd,buffer,8000,0);
        printf("Got %d byte packet\n",recv_len);
        dump(buffer,recv_len);
    }
    return 0;
}