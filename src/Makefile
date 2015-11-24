all: bidding_system host player

bidding_system: bidding_system.c utility.o
	gcc bidding_system.c utility.o -o bidding_system

host: host.c utility.o
	gcc host.c utility.o -o host

player: player.c
	gcc player.c -o player

utility.o: utility.c
	gcc -c utility.c
