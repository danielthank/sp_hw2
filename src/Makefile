all: bidding_system host

bidding_system: bidding_system.c utility
	gcc bidding_system.c utility.o -o bidding_system

host: host.c
	gcc host.c -o host

utility: utility.c
	gcc -c utility.c
