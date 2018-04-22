deck.o:	deck.c
	gcc -Wall -g -c deck.c
	
joueur.o:	joueur.h
	gcc -Wall -g -c joueur.c
	
main.o:	deck.h	joueur.h
	gcc -Wall -g -c main.c
	
lv21:	deck.o joueur.o main.o
	gcc *.o -Wall -g -o lv21

clean:
	rm *.o