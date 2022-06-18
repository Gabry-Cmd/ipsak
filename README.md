### IP Swiss Army Knife - IPSAK
This programs takes an IP address and outputs all the required informations about it.
Subnet masks are not supported for now.

The default behaviour (no arguments, only IP) is to print everything in decimal form (STD),
have a look at the usage() function for more info.

Internally, the addresses are stored in 32 bits integers.
The most significant octet is in the msb(yte) and the least significant one in the lsb(yte).

Printf and scanf vulnerabilities are not covered for now.

Born for fun at 4 am.
