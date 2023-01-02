/*
IP Swiss Army Knife - IPSAK

Gabry-Cmd

This programs takes an IP address and outputs all the required informations about it.
Subnet masks are not supported for now.

The default behaviour (no arguments, only IP) is to print everything in decimal form (DEC),
have a look at the usage() function for more info.

Printf and scanf vulnerabilities are not covered for now.
*/

#include "common.h"
#include "ipv4.h"
#include "ipv6.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// prints information about the command arguments
void usage();

int main(int argc, char * argv[])
{
	/* your code */
	int i, j; // loop counter
    ipv4_t ip = {};
    ipv6_t * ip6 = NULL;
	unsigned char x; // temp variable
    unsigned char ipv4Found = 0, subnetMaskFound = 0;
    unsigned char ipv6Found = 0;
    // the msb is used to know if the info has been requested
    // from the command line, the other bits works accordingly to
    // the IP_* defines of this program
    cmdlineflags_t flags = {};
	
	// reading arguments
    for(i=1; i<argc; i++){
        long long tempIP = 0; // IP address error checking requires a 64 bit variable
        ipv6_t * tempIpv6;
        int out = 0; // temp variable
        
        if((tempIP = decodeIpv4(argv[i])) != -1){
            if(ipv6Found){
                printf("ERROR: You can't have two separate IPV4 and IPV6\n");
                goto usage_and_help;
            }
            
            if(ipv4Found){ // there can only be one ip address
                if(subnetMaskFound){
                    goto usage_and_help;
                }
                ip.subnetMask = (int)tempIP;
                if(!isValidMaskIpv4(ip.subnetMask)){
                    goto usage_and_help;
                }
                subnetMaskFound = 1;
                continue; // skips the assignment to ip
            }
            ip.address = (int)tempIP;
            ipv4Found = 1;
        }else if((tempIpv6 = decodeIpv6(argv[i])) != NULL){
            if(ipv4Found || ipv6Found){
                printf("ERROR: You can't have two separate IPV4 and IPV6\n");
                goto usage_and_help;
            }

            ip6 = tempIpv6;
            ipv6Found = 1;
        }else if(strcmp(argv[i], "--help") == 0){ // print help and exit
            usage_and_help: // don't worry it's the only goto
            usage();
            exit(0);
        }else if(strcmp(argv[i], "--ip") == 0){
            if(i+1 < argc){ // ip without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.ip |= out;
        }else if(strcmp(argv[i], "--broadcast") == 0){
            if(i+1 < argc){ // broadcast without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.broadcast |= out;
        }else if(strcmp(argv[i], "--netmask") == 0){
            if(i+1 < argc){ // netmask without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.netmask |= out;
        }else if(strcmp(argv[i], "--subnetmask") == 0){
            if(i+1 < argc){ // subnetmask without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.subnetMask |= out;
        }else if(strcmp(argv[i], "--network") == 0){
            if(i+1 < argc){ // network without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.network |= out;
        }else if(strcmp(argv[i], "--range") == 0){
            if(i+1 < argc){ // range without flags prints DEC
                out = getFlags(argv[i+1]);
            }
            if(out == -1 || out == 0){ out = IP_DEC; }
            flags.range |= out;
        }else if(strcmp(argv[i], "--class") == 0){
            flags.class |= IP_DEC;
        }
    }
	
	// Elaboration
    // printf("FLAGS DEBUG ip %X, bc %X, mask %X, class %X, range %X\n", flagsIP, flagsBroadcast, flagsNetmask, flagsClass, flagsRange);
    
    if(!ipv4Found && !ipv6Found){
        goto usage_and_help;
    }
    // Default behaviour (no arguments, only IP)
    if(!flags.ip && !flags.class && !flags.broadcast && !flags.netmask && !flags.subnetMask && !flags.network && !flags.range){
        if(ipv4Found){
            flags.ip = IP_DEC;
            flags.class = IP_DEC;
            flags.broadcast = IP_DEC;
            flags.netmask = IP_DEC;
            flags.subnetMask = IP_DEC;
            flags.network = IP_DEC;
            flags.range = IP_DEC;
        }
        else if(ipv6Found){
            flags.ip = IP_HEX;
        }
    }

    if(ipv4Found)
        computeAndPrintIpv4(ip, flags, subnetMaskFound);
    else if(ipv6Found)
        computeAndPrintIpv6(ip6, flags);
   
    // printf("Multicast: %s\n", classIs == 'D' ? "yes" : "no");

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