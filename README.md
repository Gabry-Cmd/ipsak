### IP Swiss Army Knife - IPSAK
This programs takes an IP address and outputs all the required informations about it.
Subnet masks are not supported for now.

The default behaviour (no arguments, only IP) is to print everything in decimal form (STD),
have a look at the usage() function for more info -->
```
Takes and address and returns some informations out of it...
ipsak <address> [--ip|--broadcast|--netmask|--network|--range <flags>][--class]
Flags can be "sxobiu" (s-Standard, x-Hexadecimal, o-Octal, b-Binary, i-Integer, u-Unsigned int)
If no flags are specified everything is printed in standard form (decimal).
```

Internally, the addresses are stored in 32 bits integers.
The most significant octet is in the msb(yte) and the least significant one in the lsb(yte).

Printf and scanf vulnerabilities are not covered for now.

Born for fun at 4 am.