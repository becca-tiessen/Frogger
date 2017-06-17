prog: frogger

frogger : frogger.c llist.c player.c log.c gameglobals.c threadwrappers.c console.c
	clang -Wall -g -lcurses -pthread -o frogger *.c

clean :
	rm frogger 

