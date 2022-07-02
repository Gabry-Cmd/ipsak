/*
IP Swiss Army Knife - IPSAK

Gabry-Cmd

This programs takes an IP address and outputs all the required informations about it.
Subnet masks are not supported for now.

The default behaviour (no arguments, only IP) is to print everything in decimal form (STD),
have a look at the usage() function for more info.

Printf and scanf vulnerabilities are not covered for now.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP_STD      0x00000001  // 's'
#define IP_HEX      0x00000010  // 'x'
#define IP_OCT      0x00000100  // 'o'
#define IP_BIN      0x00001000  // 'b'
#define IP_INT      0x00010000  // 'i'
#define IP_UINT     0x00100000  // 'u'
#define IP_ALL      IP_STD | IP_HEX | IP_OCT | IP_BIN | IP_INT | IP_UINT

// takes a char and returns true if it is a digit '0'-'9'
// note that isdigit() exist in ctype.h
__attribute__((always_inline)) inline int isDigit(char c){
    if(c >= '0' && c <= '9'){
        return 1;
    }
    return 0;
}

// Takes an ip address as a string ("x.x.x.x") and returns
// a corrisponding integer form (every byte is an octect of the ip)
// Returns -1 (0xffffffffffffffff - 64 bit) if error
// cannot return -1 as int 32 bit as it corresponds to 255.255.255.255
long long decodeIP(char *);

// Takes an ip address and prints it in the form x.x.x.x
// also takes a flag to print in HEX or OCT form etc, pass 1 if not used
void printDecodedIP(int, int);

// Gets the class of an IP address as an upper case letter
char getIPClass(int);

// Gets an ip from the user by keyboard
int getIPKeyboard();

// Takes a string and returns the flags ("sxoiu")
// as a corrisponding combinations of the IP_* defines
// return -1 if error
int getFlags(char *);

// Validates a ip mask returned by decodeIP()
// Works with net masks, subnet/supernet masks
// Returns 1 if valid, 0 otherwise
int isValidMask(int);

// prints information about the command arguments
void usage();

int main(int argc, char * argv[])
{
	/* your code */
	int i, j; // loop counter
	unsigned char x; // temp variable
	int ip = 0, subnetMask = 0;
    unsigned char ipFound = 0, subnetMaskFound = 0;
    // the msb is used to know if the info has been requested
    // from the command line, the other bits works accordingly to
    // the IP_* defines of this program
    unsigned int flagsIP = 0,
                flagsClass = 0,
                flagsBroadcast = 0,
                flagsNetmask = 0,
                flagsSubnetMask = 0,
                flagsNetwork = 0,
                flagsRange = 0;
	
	// reading arguments
    for(i=1; i<argc; i++){
        long long tempIP = 0; // IP address error checking requires a 64 bit variable
        int out = 0; // temp variable
        
        if((tempIP = decodeIP(argv[i])) != -1){
            if(ipFound){ // there can only be one ip address
                if(subnetMaskFound){
                    goto usage_and_help;
                }
                subnetMask = (int)tempIP;
                if(!isValidMask(subnetMask)){
                    goto usage_and_help;
                }
                subnetMaskFound = 1;
                continue; // skips the assignment to ip
            }
            ip = (int)tempIP;
            ipFound = 1;
        }else if(strcmp(argv[i], "--help") == 0){ // print help and exit
            usage_and_help: // don't worry it's the only goto
            usage();
            exit(0);
        }else if(strcmp(argv[i], "--ip") == 0){
            if(i+1 < argc){ // ip without flags prints STD
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsIP |= out;
        }else if(strcmp(argv[i], "--broadcast") == 0){
            if(i+1 < argc){ // broadcast without flags prints std
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsBroadcast |= out;
        }else if(strcmp(argv[i], "--netmask") == 0){
            if(i+1 < argc){ // netmask without flags prints STD
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsNetmask |= out;
        }else if(strcmp(argv[i], "--subnetmask") == 0){
            if(i+1 < argc){ // subnetmask without flags prints STD
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsSubnetMask |= out;
        }else if(strcmp(argv[i], "--network") == 0){
            if(i+1 < argc){ // network without flags prints STD
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsNetwork |= out;
        }else if(strcmp(argv[i], "--range") == 0){
            if(i+1 < argc){ // range without flags prints STD
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_STD; }
            flagsRange |= out;
        }else if(strcmp(argv[i], "--class") == 0){
            flagsClass |= IP_STD;
        }
    }
	
	// Elaboration
    // Data is elaborated accordingly to the flags,
    // some data is required to calculate the others like the broadcast address
    int broadcast = 0;
    int netmask = 0;
    int network = 0;
    int rangeMin, rangeMax; // ip addresses range
    char class = 0;

    // printf("FLAGS DEBUG ip %X, bc %X, mask %X, class %X, range %X\n", flagsIP, flagsBroadcast, flagsNetmask, flagsClass, flagsRange);
    
    if(!ipFound){
        goto usage_and_help;
    }
    // Default behaviour (no arguments, only IP)
    if(ipFound && !flagsIP && !flagsClass && !flagsBroadcast && !flagsNetmask && !flagsSubnetMask && !flagsNetwork && !flagsRange){
        flagsIP = IP_STD;
        flagsClass = IP_STD;
        flagsBroadcast = IP_STD;
        flagsNetmask = IP_STD;
        flagsSubnetMask = IP_STD;
        flagsRange = IP_STD;
    }
    if(flagsIP){
        printf("IP\n");
        printDecodedIP(ip, flagsIP);
    }
    if(flagsClass || flagsBroadcast || flagsNetmask || flagsNetwork || flagsRange){
        class = getIPClass(ip);
        if(flagsClass){
            printf("CLASS\t%c\n", class);
        }
    }
    if(flagsBroadcast){
        printf("BROADCAST\n");
        if(subnetMaskFound){
            broadcast = ip | (~subnetMask);
        } else if(class == 'A'){ // A Class
            broadcast = ip | 0x00ffffff;
        } else if(class == 'B'){ // B Class
            broadcast = ip | 0x0000ffff;
        } else if(class == 'C'){ // C Class
            broadcast = ip | 0x000000ff;
        } else{
            broadcast = 0; // behaviour is still to be decided
        }
        printDecodedIP(broadcast, flagsBroadcast);
    }
    if(flagsNetmask){
        printf("NETMASK\n");
        if(class == 'A'){ // A Class
            netmask = 0xff000000;
        } else if(class == 'B'){ // B Class
            netmask = 0xffff0000;
        } else if(class == 'C'){ // C Class
            netmask = 0xffffff00;
        } else{
            netmask = 0xffffffff; // behaviour is still to be decided
        }
        printDecodedIP(netmask, flagsNetmask);
    }
    if(flagsSubnetMask){
        printf("SUBNET MASK\n");
        printDecodedIP(subnetMask, flagsSubnetMask);
    }
    if(flagsNetwork){
        printf("NETWORK\n");
        if(subnetMaskFound){
            network = ip & subnetMask;
        } else if(class == 'A'){ // A Class
            network = ip & 0xff000000;
        } else if(class == 'B'){ // B Class
            network = ip & 0xffff0000;
        } else if(class == 'C'){ // C Class
            network = ip & 0xffffff00;
        } else{
            network = ip & 0xffffffff; // behaviour is still to be decided
        }
        printDecodedIP(network, flagsNetwork);
    }
    if(flagsRange){
        // the broadcast and network ips must not be considered in the range
        if(subnetMaskFound){
            rangeMin = (ip & subnetMask) + 1;
            rangeMax = (ip | (~subnetMask) - 1);
            if(subnetMask & 2){ // if last octet is 254
                rangeMin -= 1;
                rangeMax += 1;
            }
            if(subnetMask & 1){ // if last octet is 255, behaviour is still to be decided
                rangeMin = 0;
                rangeMax = 0;
            }
        } else if(class == 'A'){ // A Class
            rangeMin = (ip & 0xff000000) + 1;
            rangeMax = (ip & 0xff000000) + 0x00fffffe;
        } else if(class == 'B'){ // B Class
            rangeMin = (ip & 0xffff0000) + 1;
            rangeMax = (ip & 0xffff0000) + 0x0000fffe;
        } else if(class == 'C'){ // C Class
            rangeMin = (ip & 0xffffff00) + 1;
            rangeMax = (ip & 0xffffff00) + 0x000000fe;
        } else{
            rangeMin = 0; // behaviour is still to be decided
            rangeMax = 0;
        }
        printf("RANGE MIN\n");
        printDecodedIP(rangeMin, flagsRange);
        printf("RANGE MAX\n");
        printDecodedIP(rangeMax, flagsRange);
    }
    
    // printf("Multicast: %s\n", class == 'D' ? "yes" : "no");

	return 0;
}

void usage(){
    puts("Takes an IP address and returns some informations out of it...\n");
    puts("ipsak <address> <subnetmask> [--ip|--broadcast|--netmask|--subnetmask|--network|--range [flags]][--class|--help]\n");
    puts("Flags can be \"asxobiu\" (a-All, s-Standard, x-Hexadecimal, o-Octal, b-Binary, i-Integer, u-Unsigned int).");
    puts("If no flags are specified everything is printed in standard form (decimal).");
    puts("If a subnet mask is provided, --ip and --network may return the same result.");
    puts("RANGE shows the usable ip addresses which don't contain network and broadcast.");
    exit(0);
}

int isValidMask(int mask){
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
            flags |= IP_STD;
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

int getIPKeyboard(){
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

char getIPClass(int ip){
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

void printDecodedIP(int ip, int flags){
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
    if(flags & IP_STD){
        printf("STD\t");
        for(i=3; i>=0; i--){
            printf("%d.", (unsigned char)(ip >> (8*i)));
        }
        printf("\b \n");
    }

    return;
}

long long decodeIP(char * str){
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