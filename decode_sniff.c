#include<stdlib.h>
#include<stdio.h>
#include<pcap.h>
#include<stdint.h>
#include<netinet/in.h>

#include "hacking.h"
#include "headers.h"

void caught_packet(unsigned char*,struct pcap_pkthdr*,const unsigned char*);
void pcap_fatal(const char*where, unsigned char *errbuf);

int main(void){
    const char *packet;
    uint8_t errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handler;
    uint8_t *device;

    device = pcap_lookupdev(errbuf);
    if(device == NULL)
        pcap_fatal("pcap_lookupdev",errbuf);
    printf("Sniffing on device %s\n",device);

    handler = pcap_open_live(device,4096,1,0,errbuf);
    if(handler == NULL)
        pcap_fatal("pcap_open_live",errbuf);
    
    pcap_loop(handler,3,caught_packet,NULL);
    pcap_close(handler);
    return 0;
}

void decode_ether(uint8_t *packet){
    /*All work is done in that line*/
    const struct ethernet_header *eth_header = (const struct ethernet_header*)packet;


    printf("[[ LAYER 2 :: Ethernet Header ]]\n");
    //Print MAC as ff:ff:ff:ff:ff:fa
    printf("[ SOURCE: %02x",eth_header->source[0]);
    for(int i=1;i<ETHER_ADD_LEN;i++){
        printf(":%02x",eth_header->source[i]);
    }
    printf("\tDESTINATION: %02x",eth_header->destination[0]);
    for(int i=1;i<ETHER_ADD_LEN;i++){
        printf(":%02x",eth_header->destination[i]);
    }
    printf("\tType: %hu ]\n",eth_header->ether_type);
}

void decode_ip(uint8_t *packet){
    const struct ip_header *ip_hdr;
    ip_hdr = (const struct ip_header*) packet;
    
    struct in_addr src;
    src.s_addr = ip_hdr->source_addr;
    struct in_addr desc;
    desc.s_addr = ip_hdr->dest_addr;

    printf("\t(( LAYER 3 ::: IP HEADER ))");
    printf("\t( Source: %s",inet_ntoa(src));
    printf("\tDest: %s)\n",inet_ntoa(desc));
    printf("\t( Type: %u\t", (uint32_t) ip_hdr->type_of_service);
    printf("ID: %hu\tLength: %hu )\n", ntohs(ip_hdr->identification), ntohs(ip_hdr->total_len));
}

uint32_t decode_tcp(uint8_t *packet){
    uint32_t header_size;
    const struct tcp_header *header;
    header = (const struct tcp_header*)packet;

    header_size = 4*header->offset;

    printf("\t\t{{ Layer 4 :::: TCP Header }}\n");
    printf("\t\t{ Src Port: %hu\t", ntohs(header->src_port));
    printf("Dest Port: %hu }\n", ntohs(header->dest_port));
    printf("\t\t{ Seq #: %u\t", ntohl(header->seq_num));
    printf("Ack #: %u }\n", ntohl(header->ack_num));
    printf("\t\t{ Header Size: %u\tFlags: ", header_size);

    if(header->flags & TCP_FIN)
        printf("FIN ");
    if(header->flags & TCP_SYN)
        printf("SYN ");
    if(header->flags & TCP_RST);
        printf("RST ");
    if(header->flags & TCP_PUSH)
        printf("PUSH ");
    if(header->flags & TCP_ACK)
        printf("ACK ");
    if(header->flags & TCP_URG)
        printf("URG ");


    printf("}\n");

    return header_size;
}

void caught_packet(uint8_t *user_args, struct pcap_pkthdr *header, const uint8_t *packet){

    int tcp_header_len,total_header_size,ip_hdr_len,packet_data_len;
    uint8_t *packet_data;

    printf("========Got %d byte packet=======\n",header->len);

    decode_ether(packet);
    decode_ip(packet+ETHER_HEADER_SIZE);

    ip_hdr_len = sizeof(struct ip_header);
    
    tcp_header_len = decode_tcp(packet+ETHER_HEADER_SIZE+ip_hdr_len);

    total_header_size = tcp_header_len + ip_hdr_len + ETHER_HEADER_SIZE;

    /*Get packet data, exclude headers*/
    packet_data = (uint8_t*)packet+total_header_size;
    packet_data_len = header->len - total_header_size;

    if(packet_data_len > 0){
        printf("\t\t\t%u bytes of data\n",packet_data_len);
        dump(packet_data,packet_data_len);
    }else{
        printf("\t\t\tNo packet data\n");
    }
}

void pcap_fatal(const char*where, unsigned char *errbuf){
    printf("Fatal error in %s: %s\n",where,errbuf);
    exit(-1);
}