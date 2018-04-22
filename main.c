#include <stdlib.h>
#include <stdio.h>
#include "deck.h"
#include "joueur.h"

int main(){
	//Test des fonctions de joueur.h
	initDeckLib();
	decktype_t type = P52;
	int nb_players = 3;
	int nb_deck = 8;
	int nb_hand = 40;
	BANK b = init_bank(b,type,nb_players,nb_deck,nb_hand);
	int i;
	PLAYER p1 = init_joueur(p1);
	PLAYER p2 = init_joueur(p2);
	PLAYER p3 = init_joueur(p3);
	
	for(i=0;i<2;i++){
		b.cards[i] = drawCard(b.deck);
		b.nb_cards++;
		p1.cards[i] = drawCard(b.deck);
		p1.nb_cards++;
		p2.cards[i] = drawCard(b.deck);
		p2.nb_cards++;
		p3.cards[i] = drawCard(b.deck);
		p3.nb_cards++;
	}
	
	b = total_carte_bank(b);
	p1 = total_carte(p1);
	p2 = total_carte(p2);
	p3 = total_carte(p3);
	
	printCard(b.cards[0]);
	printf(" ");
	printCard(b.cards[1]);
	printf(" ");
	printf("%d\n",b.tot_cards);
	
	printCard(p1.cards[0]);
	printf(" ");
	printCard(p1.cards[1]);
	printf(" ");
	printf("%d\n",p1.tot_cards);
	
	printCard(p2.cards[0]);
	printf(" ");
	printCard(p2.cards[1]);
	printf(" ");
	printf("%d\n",p2.tot_cards);
	
	printCard(p3.cards[0]);
	printf(" ");
	printCard(p3.cards[1]);
	printf(" ");
	printf("%d\n",p3.tot_cards);
	
	exit(0);
}
