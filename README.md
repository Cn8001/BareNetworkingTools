## Some networking tools from the book Hacking: The Art of Exploitination 

**Simple Server:**   Creates a server with sockets, used for communication<br>
Compile command >> gcc simple_server.c hacking.c -Iinclude/ -o simple_server

**Host lookup:**      Used for getting IP from hostname.<br>
Compile command >> gcc host_lookup.c hacking.c -Iinclude/ -o host_lookup

**Raw sniffer:**      Raw socket sniffer (TCP)<br>
Compile command >> gcc raw_sniffer.c hacking.c -Iinclude/ -o rawsock_sniffer

**Pcap sniff:**       TCP sniffer, used pcap.<br>
Compile command >> gcc pcap_sniff.c hacking.c -Iinclude/ -lpcap -o pcap_sniff
* It requires to link with pcap.

**Webserver ID:**     Gets IP from hostname and sends a HEAD request to get webserver type.<br>
Compile command >> gcc webserver_id.c hacking.c -Iinclude/ -o webserver_id

**Sniff Decoder:**    Listens in promiscuous mode, decodes ethernet,IP and TCP headers. Used pcap to sniff.<br>
Compile command >> gcc decode_sniff.c hacking.c -Iinclude/ -lpcap -o decode_sniff
* Requires to link with pcap. Also, TCP,IP and Ethernet headers are defined in include/headers.h

**SYNFLOOD:**         Simple SYN packet flooding tool. Used libnet to generate flood packages.<br>
Compile command >> gcc synflood_libnet.c -Iinclude/ -lnet -o simple_server
* Requires libnet.
* The book was written for libnet 1.0, switched to newer version of libnet.
