#include "ipv4.h"
#include "common.h"
#include <string.h>
#include <stdio.h>

int isValidMaskIpv4(int mask){
    int i, oneFound = 0;

    for(i=0; i<32; i++){ // right to left
        if((mask >> i) & 1){
            oneFound = 1;
        }
        else if(oneFound){ // if a zero found is next to a one
            return 0;
        }
    }
    return 1;
}

int getFlags(char * str){
    int i;
    int flags = 0;

    if(strlen(str) <= 0){
        return -1;
    }
    for(i=0; i<strlen(str); i++){
        if(str[i] == 'a'){
            flags |= IP_ALL;
        }else if(str[i] == 's'){
            flags |= IP_DEC;
        }else if(str[i] == 'x'){
            flags |= IP_HEX;
        }else if(str[i] == 'o'){
            flags |= IP_OCT;
        }else if(str[i] == 'b'){
            flags |= IP_BIN;
        }else if(str[i] == 'i'){
            flags |= IP_INT;
        }else if(str[i] == 'u'){
            flags |= IP_UINT;
        }else{ return -1; }
    }

    return flags;
}

int getKeyboardIpv4(){
    int ip, i;
    unsigned char x;

    for(i=3; i>=0; i--){
        do{
            printf("Octect [%d]:", i);
            scanf("%d", &x);
        } while(x < 0 || x > 255);
        
        ip += x << (8*i);
    }

    return ip;
}

char getClassIpv4(int ip){
    char class;
    unsigned char msoctect = (ip >> 24);

    if(0 <= msoctect && msoctect <= 127){ // A Class
        class = 'A';
    } else if(128 <= msoctect && msoctect <= 191){ // B Class
        class = 'B';
    } else if(192 <= msoctect && msoctect <= 223){ // C Class
        class = 'C';
    } else if(224 <= msoctect && msoctect <= 239){ // D Class
        class = 'D';
    } else if(240 <= msoctect && msoctect <= 255){ // E Class - Reserved
        class = 'E';
    }

    return class;
}

void printDecodedIpv4(int ip, int flags){
    int i;

    if(flags & IP_INT){
        printf("INT\t%d\n", (int)ip);
    }
    if(flags & IP_UINT){
        printf("UINT\t%u\n", ip);
    }
    if(flags & IP_HEX){
        printf("HEX\t");
        for(i=3; i>=0; i--){
            printf("%X.", (unsigned char)(ip >> (8*i)));
        }
        printf("\b \n");
    }
    if(flags & IP_OCT){
        printf("OCT\t");
        for(i=3; i>=0; i--){
            printf("%o.", (unsigned char)(ip >> (8*i)));
        }
        printf("\b \n");
    }
    if(flags & IP_BIN){
        printf("BIN\t");
        for(i=31; i>=0; i--){
            printf("%d", (unsigned char)(ip >> (i)) & 1);
            if(i % 8 == 0 && i != 0){
                putc('.', stdout);
            }
        }
        printf("\n");
    }
    if(flags & IP_DEC){
        printf("DEC\t");
        for(i=3; i>=0; i--){
            printf("%d.", (unsigned char)(ip >> (8*i)));
        }
        printf("\b \n");
    }

    return;
}

long long decodeIpv4(char * str){
    int i, offset = 24; // counters
    long long octet = 0; // current octet
    long long ip = 0;

    // The ip variable is filled left to right
    // The max ip lenght is 16 chars with the terminator
    for(i=0; i<16; i++){
        if(isDigit(str[i])){
            octet = octet*10 + (str[i] - '0');
            if(octet > 255){ return -1; }
        }else if(str[i] == '.'){
            ip += (octet << offset);
            octet = 0;
            offset -= 8; // 8 bits to the right
        }else if(str[i] == 0 || str[i] == 13 || str[i] == 10){ // NULL || CR || LF
            ip += octet;
            break;
        }
    }

    if(offset == 0){
        return ip;
    }
    else{
        return -1;
    }
}

void computeAndPrintIpv4(ipv4_t ip, cmdlineflags_t flags, unsigned char subnetMaskFound){
    // Data is elaborated accordingly to the flags,
    // some data is required to calculate the others like the broadcast address
    if(flags.ip){
        printf("IPV4\n");
        printDecodedIpv4(ip.address, flags.ip);
    }
    if(flags.class || flags.broadcast || flags.netmask || flags.network || flags.range){
        ip.classIs = getClassIpv4(ip.address);
        if(flags.class){
            printf("CLASS\t%c\n", ip.classIs);
        }
    }
    if(flags.broadcast || flags.range){
        printf("BROADCAST\n");
        if(subnetMaskFound){
            ip.broadcast = ip.address | (~ip.subnetMask);
        } else if(ip.classIs == 'A'){ // A Class
            ip.broadcast = ip.address | 0x00ffffff;
        } else if(ip.classIs == 'B'){ // B Class
            ip.broadcast = ip.address | 0x0000ffff;
        } else if(ip.classIs == 'C'){ // C Class
            ip.broadcast = ip.address | 0x000000ff;
        } else{
            ip.broadcast = 0; // behaviour is still to be decided
        }
        printDecodedIpv4(ip.broadcast, flags.broadcast);
    }
    if(flags.netmask){
        printf("NETMASK\n");
        if(ip.classIs == 'A'){ // A Class
            ip.netmask = 0xff000000;
        } else if(ip.classIs == 'B'){ // B Class
            ip.netmask = 0xffff0000;
        } else if(ip.classIs == 'C'){ // C Class
            ip.netmask = 0xffffff00;
        } else{
            ip.netmask = 0xffffffff; // behaviour is still to be decided
        }
        printDecodedIpv4(ip.netmask, flags.netmask);
    }
    if(flags.subnetMask){
        printf("SUBNET MASK\n");
        printDecodedIpv4(ip.subnetMask, flags.subnetMask);
    }
    if(flags.network || flags.range){
        printf("NETWORK\n");
        if(subnetMaskFound){
            ip.network = ip.address & ip.subnetMask;
        } else if(ip.classIs == 'A'){ // A Class
            ip.network = ip.address & 0xff000000;
        } else if(ip.classIs == 'B'){ // B Class
            ip.network = ip.address & 0xffff0000;
        } else if(ip.classIs == 'C'){ // C Class
            ip.network = ip.address & 0xffffff00;
        } else{
            ip.network = ip.address & 0xffffffff; // behaviour is still to be decided
        }
        printDecodedIpv4(ip.network, flags.network);
    }
    if(flags.range){
        // the broadcast and network ips must not be considered in the range
        if(subnetMaskFound){
            ip.rangeMin = (ip.address & ip.subnetMask);
            ip.rangeMax = (ip.address | (~ip.subnetMask));
            if(ip.subnetMask & 2){ // if last octet is 254
                ip.rangeMin = ip.network;
                ip.rangeMax = ip.broadcast;
            }
            if(ip.subnetMask & 1){ // if last octet is 255, behaviour is still to be decided
                ip.rangeMin = 0;
                ip.rangeMax = 0;
            }
        } else if(ip.classIs == 'A'){ // A Class
            ip.rangeMin = (ip.address & 0xff000000);
            ip.rangeMax = (ip.address & 0xff000000) + 0x00ffffff;
        } else if(ip.classIs == 'B'){ // B Class
            ip.rangeMin = (ip.address & 0xffff0000);
            ip.rangeMax = (ip.address & 0xffff0000) + 0x0000ffff;
        } else if(ip.classIs == 'C'){ // C Class
            ip.rangeMin = (ip.address & 0xffffff00);
            ip.rangeMax = (ip.address & 0xffffff00) + 0x000000ff;
        } else{
            ip.rangeMin = 0; // behaviour is still to be decided
            ip.rangeMax = 0;
        }
        printf("RANGE MIN\n");
        printDecodedIpv4(ip.rangeMin, flags.range);
        printf("RANGE MAX\n");
        printDecodedIpv4(ip.rangeMax, flags.range);
    }
}