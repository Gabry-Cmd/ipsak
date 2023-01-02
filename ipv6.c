#include "ipv6.h"
#include "common.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

ipv6_t * decodeIpv6(char * str){
    int i = 0;
    int word = 0;
    int wordOff = 0;
    unsigned int error = 0;
    ipv6_t * ip = (ipv6_t *)malloc(sizeof(ipv6_t));

    // invalid string
    if(str == (void *)0 || str[0] == '\0'){
        error = 1;
    }

    while(!error){
        char r = asciiHex2Bin(str[i]);
        if(r != -1){ // Is a number?
            word = word*16 + r;
            if(word > USHRT_MAX){
                error = 1;
            }
        }
        else{ // Is a divisor or are done?
            if(str[i] == ':' || str[i] == '/'){
                ip->address[wordOff] = word;
                wordOff++;
                word = 0;
            }else if(str[i] == '\0'){ // exit
                if(wordOff == 7){
                    ip->address[wordOff] = word;
                    break;
                }
                    
                error = 1;
            }
        }
        i++;
    }

    if(error){
        free(ip);
        return (void *)0;
    }
    return ip;
}

void printDecodedIpv6(ipv6_t * ip, int flags){
    int i,j;

    // printf()' default integer promotion makes this feature unfeasible to do with it
    // if(flags & IP_INT){
    //     printf("INT\t");
    //     for(i=0; i<8; i++){
    //         printf("%d", ip->address[i]);
    //         if(i != 7)
    //             putc(':', stdout);
    //     }
    //     putc('\n', stdout);
    // }
    if(flags & IP_UINT){
        printf("UINT\t");
        for(i=0; i<8; i++){
            printf("%u", ip->address[i]);
            if(i != 7)
                putc(':', stdout);
        }
        putc('\n', stdout);
    }
    if(flags & IP_HEX){
        printf("HEX\t");
        for(i=0; i<8; i++){
            printf("%X", (unsigned short)(ip->address[i]));
            if(i != 7)
                putc(':', stdout);
        }
        putc('\n', stdout);
    }
    if(flags & IP_OCT){
        printf("OCT\t");
        for(i=0; i<8; i++){
            printf("%o", (unsigned short)(ip->address[i]));
            if(i != 7)
                putc(':', stdout);
        }
        putc('\n', stdout);
    }
    if(flags & IP_BIN){
        printf("BIN\t");
        for(i=0; i<8; i++){
            for(j=15; j>=0; j--){
                printf("%d", (unsigned short)(ip->address[i] >> (j)) & 1);
            }
            if(i != 7)
                putc(':', stdout);
        }
        putc('\n', stdout);
    }
    if(flags & IP_DEC){
        printf("DEC\t");
        for(i=0; i<8; i++){
            printf("%d", (unsigned short)(ip->address[i]));
            if(i != 7)
                putc(':', stdout);
        }
        putc('\n', stdout);
    }
    
    return;
}

void computeAndPrintIpv6(ipv6_t * ip, cmdlineflags_t flags){
    if(flags.ip){
        printf("IPV6\n");
        printDecodedIpv6(ip, flags.ip);
    }
}