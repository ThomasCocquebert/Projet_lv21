deck.o:	deck.c
	gcc -Wall -g -c deck.c
	
joueur.o:	joueur.h joueur.c
	gcc -Wall -g -c joueur.c
	
main.o:	deck.h joueur.h main.c
	gcc -Wall -g -lpthread -c main.c
	
lv21:	deck.o joueur.o main.o
	gcc *.o -Wall -g -lpthread -o lv21

clean:
	rm *.o