# IPSAK - IP Swiss Army Knife

## Purpose
This programs takes an IP address and outputs all the required informations about it.
Subnet masks are not supported for now.

## General Infos
The default behaviour (no arguments, only IP) is to print everything in decimal form (STD),
have a look at the usage() function for more info -->
```
Takes and address and returns some informations out of it...
ipsak <address> [--ip|--broadcast|--netmask|--subnetmask|--network|--range [flags]][--class]
Flags can be "sxobiu" (s-Standard, x-Hexadecimal, o-Octal, b-Binary, i-Integer, u-Unsigned int)
If no flags are specified everything is printed in standard form (decimal).
If a subnet mask is provided, --ip and --network may return the same result.
```

Internally, the addresses are stored in 32 bits integers.
The most significant octet is in the msb(yte) and the least significant one in the lsb(yte).

Printf and scanf vulnerabilities are not covered for now.

## Compiling
Compiling is simple enough, here there are two templates (I'm using gcc 10 or Mingw 8)

Debug
```
gcc ipsak.c -o bin/ipsak -g
```
Release
```
gcc ipsak.c -o bin/ipsak -O3 -s
```

## Contribution
I'm going to write the guidelines as soon as possibile, for now:
You can open an issue on the GitHub issue tracker or open a pull request.
If you want to make huge modification to the source you may need to consider forking,

## Why Though?
Born for fun at 4 am.