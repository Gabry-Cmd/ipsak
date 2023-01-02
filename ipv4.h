/*
IP Swiss Army Knife - IPSAK

Gabry-Cmd

This file contains all the IPv4 related functionalities of the program
*/

#ifndef IPV4_IPSAK_H
#define IPV4_IPSAK_H
#include "common.h"

// Takes an ip address as a string ("x.x.x.x") and returns
// a corrisponding integer form (every byte is an octect of the ip)
// Returns -1 (0xffffffffffffffff - 64 bit) if error
// cannot return -1 as int 32 bit as it corresponds to 255.255.255.255
long long decodeIpv4(char *);

// Takes an ip address and prints it in the form "x.x.x.x"
// also takes a flag to print in HEX or OCT form etc, pass IP_DEC if not used
void printDecodedIpv4(int, int);

// Gets the class of an IP address as an upper case letter
char getClassIpv4(int);

// Gets an ip from the user by keyboard
int getKeyboardIpv4();

// Takes a string and returns the flags ("sxoiu")
// as a corrisponding combinations of the IP_* defines
// return -1 if error
int getFlags(char *);

// Validates a ip mask returned by decodeIP()
// Works with net masks, subnet/supernet masks
// Returns 1 if valid, 0 otherwise
int isValidMaskIpv4(int);

// Computes all the data from the ip.address and stores it in the
// ipv4_t structures, if the last argument is != 0 the ip.subnetMask
// is used in the calculations
// Takes the ip data, flags from terminal, flag if subnetmask is found
void computeAndPrintIpv4(ipv4_t, cmdlineflags_t, unsigned char);

#endif // IPV4_IPSAK_H