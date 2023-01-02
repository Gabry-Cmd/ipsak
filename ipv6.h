/*
IP Swiss Army Knife - IPSAK

Gabry-Cmd

This file contains all the IPv6 related functionalities of the program
*/

#ifndef IPV6_IPSAK_H
#define IPV6_IPSAK_H
#include "common.h"

// Takes an ip address as a string ("y.y.y.y.y.y.y.y") and returns
// it in a ipv6_t structure
// Returns -1 if error
ipv6_t * decodeIpv6(char *);

// Takes an ip address from an ipv6_t structure and prints it in the form "y.y.y.y.y.y.y.y"
// also takes a flag to print in HEX or OCT form etc, pass IP_HEX for ipv6 if not used
void printDecodedIpv6(ipv6_t *, int);

// Computes all the data from the ip.address and stores it in the
// ipv6_t structures
// Takes the ip data and flags from terminal
void computeAndPrintIpv6(ipv6_t *, cmdlineflags_t);

#endif // IPV6_IPSAK_H