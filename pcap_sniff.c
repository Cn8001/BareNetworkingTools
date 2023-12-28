#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>
#include "hacking.h"

void pcap_fatal(const char *failed_in, const char *errbuf){
    printf("Fatal error in %s: %s\n",failed_in,errbuf);
    exit(-1);
}

int main(void){
    struct pcap_pkthdr header;
    const unsigned char *packet;
    char errbuf[PCAP_ERRBUF_SIZE];
    char *device;
    pcap_t *pcap_handle;

    device = pcap_lookupdev(errbuf);
    if(device == NULL)
        pcap_fatal("lookup_dev",errbuf);

    printf("Sniffing on device: %s\n",device);

    pcap_handle = pcap_open_live(device,4096,1,0,errbuf);
    if(pcap_handle == NULL)
        pcap_fatal("pcap_handle",errbuf);

    for(int i=0;i<3;i++){
        packet = pcap_next(pcap_handle,&header);
        printf("Captured %d byte packet\n",header.len);
        dump(packet,header.len);
    }
    pcap_close(pcap_handle);
}