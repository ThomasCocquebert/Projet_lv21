#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "deck.h"
#include "joueur.h"

void carac_joueur(PLAYER *p){
		p->cards[0] = 0;
		p->nb_cards = 0;
		p->tot_cards = 0;
		p->jetons = 1000;
		p->valStop = 18;
		p->obj_jetons = 2000;
		p->type_mise = 0;
	}
	
void *add_joueur(void *arg){
	PLAYER *p = malloc(sizeof(PLAYER));
	p = init_joueur(p);
	printf("%p\n",p);
	printf("joueur créé !\n\n");
	carac_joueur(p);
	return p;
}
	
void *demande_carte(void *arg){
	
}
	
void *player(void *arg){
	/*
	PLAYER *p = malloc(sizeof(PLAYER));
	p = init_joueur(p);
	printf("Joueur créé\n");
	carac_joueur(p);
	return NULL;
	*/
	pthread_t tidcarte;
	EXDATA *exdata = arg;
	exdata = init_data(exdata);
	printf("%d \n",exdata->nb_cards);
	return NULL;
}

int main(){
	
	srand (time(NULL));
	decktype_t type = P52;
	int nb_players = 5;
	int nb_deck = 8;
	int nb_hand = 40;
	int i;
	BANK b = init_bank(b,type,nb_players,nb_deck,nb_hand);
	pthread_t *tid = NULL;
	tid = malloc(nb_players*sizeof(pthread_t));
	EXDATA *exdata = NULL;
	exdata = malloc(nb_players*sizeof(EXDATA));
	for(i=0;i<nb_players;i++){
		pthread_create(&tid[i],NULL,player,&exdata[i]);
	}
	for(i=0;i<nb_players;i++){
		pthread_join(tid[i],NULL);
	}
	
	
	//Test avec les joueurs dans le threads principal
	/*
	PLAYER **p = NULL;
	p = malloc(nb_players*sizeof(PLAYER *));
	int i;
	for(i=0;i<5;i++){
		pthread_create(&tid[i],NULL,add_joueur,NULL);
	}
	
	for(i=0;i<5;i++){
		pthread_join(tid[i],(void *) &p[i]);
	}
	
	for(i=0;i<5;i++){
		printf("joueur %d :\n",i+1);
		printf("valeur nb_cards %d\n",p[i]->nb_cards);
		printf("valeur cards[0] %d\n",p[i]->cards[0]);
		printf("adresse %p\n\n",p[i]);
	}
	
	for(i=0;i<5;i++){
		libere_joueur(p[i]);
	}
	*/
	
	libere_bank(b);
	//free(p);
	free(tid);
	free(exdata);
	
	//Retraviller la fonction
	//libere_mem(nb_players,p,tid,b);
	exit(0);
	
}