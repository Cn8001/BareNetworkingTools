#ifndef HACKING_NETWORK_H
#define HACKING_NETWORK_H

int send_string(int sockfd,unsigned char *buffer){
    int sent_bytes,bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send > 0){
        sent_bytes = send(sockfd,buffer,bytes_to_send,0);
        if(sent_bytes == -1)
            return 0;
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1;
}

int recv_line(int sockfd,unsigned char *out_buffer){
    #define EOL "\r\n"
    #define EOL_SIZE 2

    unsigned char *ptr = out_buffer;
    int eol_match =0;
    while(recv(sockfd,ptr,1,0) == 1){
        if(*ptr == EOL[eol_match]){
            eol_match++;
            if(eol_match == EOL_SIZE){
                //Terminate the string
                *(ptr+1-EOL_SIZE) = '\0';
                return strlen(out_buffer);
            }
        }else{
            eol_match =0;
        }
        ptr++;
    }
    return 0;

}
#endif