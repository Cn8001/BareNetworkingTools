#ifndef HEADERS_H
#define HEADERS_H

#define ETHER_ADD_LEN 6
#define ETHER_HEADER_SIZE 14
/*Data link layer - ARP*/
struct ethernet_header{
    unsigned char source[ETHER_ADD_LEN];
    unsigned char destination[ETHER_ADD_LEN];
    unsigned short ether_type;
};

/*Network layer - ICMP*/
struct ip_header{
    unsigned char version_and_headerlen;
    unsigned char type_of_service;
    unsigned short total_len;
    unsigned short identification; // Identification number
    unsigned short flags_and_fragment_offset; //First 4 bytes are flags
    unsigned char time_to_live;
    unsigned char protocol;
    unsigned short checksum;
    unsigned int source_addr;
    unsigned int dest_addr;
};

/*Transport layer - TCP*/
struct tcp_header{
    unsigned short src_port;
    unsigned short dest_port;
    unsigned int seq_num;
    unsigned int ack_num;
    unsigned char reserved:4;   // 4 bits from the 6 bits of reserved space
    unsigned char offset:4;
    unsigned char flags;
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PUSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20   
    unsigned short window;  // TCP window size
    unsigned short checksum;
    unsigned short urgent;
};
#endif