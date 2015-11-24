all: bidding_system host

bidding_system: bidding_system.c utility.o
	gcc bidding_system.c utility.o -o bidding_system

host: host.c
	gcc host.c utility.o -o host

utility.o: utility.c
	gcc -c utility.c
