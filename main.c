#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "deck.h"
#include "joueur.h"

int main(){
	//Test des fonctions de joueur.h
	/*initDeckLib();
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
	
	libere_joueur(p1);
	libere_joueur(p2);
	libere_joueur(p3);
	libere_bank(b);
	*/
	void *add_joueur(void *arg){
		printf("thread en cours d'exécution !\n");
		PLAYER *p = malloc(sizeof(PLAYER));
		p = init_joueur(p);
		p->nb_cards += rand()%10+1;
		p->cards[0] = 42;
		printf("%p\n\n",p);
		printf("fin du thread\n");
		return p;
	}
	
	srand (time(NULL));
	pthread_t tid[5];
	PLAYER *p[5];
	int i;
	for(i=0;i<5;i++){
		pthread_create(&tid[i],NULL,add_joueur,NULL);
	}
	
	for(i=0;i<5;i++){
		pthread_join(tid[i],(void *) &p[i]);
	}
	
	for(i=0;i<5;i++){
		printf("valeur nb_cards %d\n",p[i]->nb_cards);
		printf("valeur cards|0] %d\n",p[i]->cards[0]);
		printf("adresse %p\n\n",p[i]);
	}
	for(i=0;i<5;i++){
		libere_joueur(p[i]);
	}	
	exit(0);
}
