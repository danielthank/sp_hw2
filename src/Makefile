all: bidding_system host player player_bonus

bidding_system: bidding_system.c utility.o
	gcc bidding_system.c utility.o -o bidding_system

host: host.c utility.o
	gcc host.c utility.o -o host

player: player.c
	gcc player.c -o player

player_bonus: player_bonus.c
	gcc player_bonus.c -o player_bonus

utility.o: utility.c
	gcc -c utility.c
