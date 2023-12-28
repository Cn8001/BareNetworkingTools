#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "hacking.h"

void dump(const unsigned char* data, const unsigned int len){
    for(int i=0;i<len;i++){
        unsigned char byte = data[i];
        printf("%02x ",byte);
        if(i%16 == 15 || i == len-1){
            for(int j=0;j<15-(i%16);j++)
                printf(" ");
            printf("| ");
            for(int j=(i-(i%16));j<=i;j++){
                byte = data[j];
                if(byte > 31 && byte < 127)
                    printf("%c",byte);
                else
                    printf(".");
            }
            printf("\n");

        }
    }
}

void fatal(const char *message){
    char buffer[1024];
    strcpy(buffer,"[!!] Fatal Error: ");
    strncat(buffer,message,81);
    perror(buffer);
    exit(-1);
}