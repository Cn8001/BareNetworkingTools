#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<netdb.h>

#include "hacking-network.h"
#include "hacking.h"

int main(int argc, char *argv[]){
    struct hostent *host;
    struct in_addr *addr;

    if(argc < 2){
        printf("Usage: %s <hostname>\n",argv[0]);
        exit(0);
    }
    host = gethostbyname(argv[1]);
    if(host == NULL)
        printf("Cannot get IP of %s\n",argv[1]);
    else{
        addr = (struct in_addr*) host->h_addr_list[0];
        printf("%s: %s\n",argv[1],inet_ntoa(*addr));

    }
}

