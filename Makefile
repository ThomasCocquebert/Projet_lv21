deck.o:	deck.c
	gcc -Wall -g -c deck.c
	
joueur.o:	joueur.h joueur.c
	gcc -Wall -g -c joueur.c
	
newmain.o:	deck.o joueur.o newmain.c
	gcc -Wall -g -c newmain.c -lpthread
	

newmain:	deck.o joueur.o newmain.o
	gcc deck.o joueur.o newmain.o -Wall -g -o newmain -lpthread
	
run_newmain:	
	./newmain
mem_run_newmain:
	valgrind ./newmain
	
clean:
	rm *.o
	
indent: joueur.c newmain.c
	indent -linux joueur.c
	indent -linux newmain.c