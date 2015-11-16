all: bidding_system

bidding_system: bidding_system.c utility
	gcc bidding_system.c utility.o -o bidding_system

utility: utility.c
	gcc -c utility.c
