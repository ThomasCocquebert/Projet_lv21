#include <stdlib.h>
#include <stdio.h>
#include "deck.h"
#include "joueur.h"
	
int main(){
	initDeckLib();
	deck_t *deck = initDeck(P54,1);
	PLAYER p1 = init_joueur(p1);
	int i;
	for(i=0;i<2;i++){
		p1.cards[i] = drawCard(deck);
		p1.nb_cards++;
	}
	p1 = total_carte(p1);
	
	for(i=0;i<2;i++){
		printCard(p1.cards[i]);
		printf(" ");
	}
	printf("%d\n",p1.tot_cards);
	exit(0);
}
