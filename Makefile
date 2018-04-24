deck.o:	deck.c
	gcc -Wall -g -c deck.c
	
joueur.o:	joueur.h joueur.c
	gcc -Wall -g -c joueur.c
	
main.o:	deck.h joueur.h main.c
	gcc -Wall -g -lpthread -c main.c
	
lv21:	deck.o joueur.o main.o
	gcc deck.o joueur.o main.o -Wall -g -lpthread -o lv21

clean:
	rm *.o
	
sandbox.o:	deck.h joueur.h sandbox.c
	gcc -Wall -g -lpthread -c sandbox.c
	
sandbox:	deck.o joueur.o sandbox.o sandbox.c
	gcc deck.o joueur.o sandbox.o -Wall -g -o sandbox -lpthread
	
oldmain.o: deck.h joueur.h old_main.c
	gcc -Wall -g -lpthread -c old_main.c

oldversion:	deck.o joueur.o old_main.o
	gcc deck.o joueur.o old_main.o -Wall -g -lpthread -o oldversion