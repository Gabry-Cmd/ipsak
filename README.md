# IPSAK - IP Swiss Army Knife

## Purpose
This programs takes an IP address, his subnet mask and output all the required informations (like the class, broadcast and network address etc) about it in various formats (OCT, HEX, DEC etc).
* **IPv4 addresses and subnet masks are supported.**
* **CIDR notation for subnet masks and IPv6 addresses are NOT supported for now.**
* **Supernet masks are NOT EXPLICITLY supported.**

## General Infos
The default behaviour (no arguments, only IP) is to print everything in decimal form (DEC),
have a look at the usage() function for more info -->
```
Takes an IP address and returns some informations out of it...
ipsak <address> [--ip|--broadcast|--netmask|--subnetmask|--network|--range [flags]][--class]
Flags can be "asxobiu" (a-All, s-Standard, x-Hexadecimal, o-Octal, b-Binary, i-Integer, u-Unsigned int)
If no flags are specified everything is printed in standard form (decimal).
If a subnet mask is provided, --ip and --network may return the same result.
RANGE shows every ip addresses including the network and broadcast ones.
```

Internally, the addresses are stored in 32 bits integers.
The most significant octet is in the msb(yte) and the least significant one in the lsb(yte).

Printf and scanf vulnerabilities are not covered for now.

## Compiling
**Compiling is simple enough, here there are two templates (I'm using gcc 10 or Mingw 8)**

Debug
```
gcc ipsak.c -o bin/ipsak -g
```
Release
```
gcc ipsak.c -o bin/ipsak -O3 -s
```

## Contribution
[CONTRIBUTING](./CONTRIBUTING.md)

## Why Though?
Born for fun at 4 am.
