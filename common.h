#ifndef COMMON_IPSAK_H
#define COMMON_IPSAK_H

#define IP_DEC      0x00000001  // 's'
#define IP_HEX      0x00000010  // 'x'
#define IP_OCT      0x00000100  // 'o'
#define IP_BIN      0x00001000  // 'b'
#define IP_INT      0x00010000  // 'i'
#define IP_UINT     0x00100000  // 'u'
#define IP_ALL      IP_DEC | IP_HEX | IP_OCT | IP_BIN | IP_INT | IP_UINT  // 'a'

typedef struct S_CmdLineFlags{
    unsigned int ip;
    unsigned int class;
    unsigned int broadcast;
    unsigned int netmask;
    unsigned int subnetMask;
    unsigned int network;
    unsigned int range;
} cmdlineflags_t;

typedef struct S_Ipv4{
    int address;
    int subnetMask;
    int broadcast;
    int netmask;
    int network;
    int rangeMin, rangeMax; // ip addresses range
    char classIs; // class is a reserved word in C++
} ipv4_t;

typedef struct S_Ipv6{
    unsigned short address[8];
} ipv6_t;

// takes a char and returns true if it is a digit '0'-'9'
// note that isdigit() exist in ctype.h
__attribute__((always_inline)) inline int isDigit(char c){
    if(c >= '0' && c <= '9'){
        return 1;
    }
    return 0;
}

// Takes and hex value from an ascii char and
// returns the corresponding binary value (4bit)
// returns -1 if error
// Is case insensitive
__attribute__((always_inline)) inline char asciiHex2Bin(char c){
    if(c >= 'a' && c <= 'f')
        c -= 32;
    return (c >= '0' && c <= '9') ? c-'0' :
            ((c >= 'A' && c <= 'F') ? c-'A'+10 : -1);
}

#endif // COMMON_IPSAK_H