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
	EXDATA *exdata = arg;
	int i;
	printf("nb_cards player %d\n",exdata->nb_cards);
	//pthread_mutex_lock(&exdata->mut);
	pthread_cond_wait(&exdata->cond,&exdata->mut);
	printf("Carte n°1: ");
	printCard(exdata->cards[0]);
	printf("\n");
	//pthread_mutex_unlock(&exdata->mut);
	//exdata->nb_cards = 42;
	//PLAYER *p = malloc(sizeof(PLAYER));
	//p = init_joueur(p);
	//pthread_cond_wait(&exdata->cond,&exdata->mut);
	//sleep(5);
	/*for(i=0;i<exdata->nb_cards;i++){
		printCard(exdata->cards[i]);
		printf("\n");
	}
	*/
	return NULL;
}

int main(){
	
	srand (time(NULL));
	decktype_t type = P52;
	int nb_players = 5;
	int nb_deck = 8;
	int nb_hand = 40;
	int i,j;
	BANK b = init_bank(b,type,nb_players,nb_deck,nb_hand);
	pthread_t *tid = NULL;
	tid = malloc(nb_players*sizeof(pthread_t));
	EXDATA *exdata = NULL;
	exdata = malloc(nb_players*sizeof(EXDATA));
	/*
	for(i=0;i<nb_players;i++){
		init_data(&exdata[0]);
		pthread_create(&tid[i],NULL,player,&exdata[i]);
	}
	for(j=0;i<2;j++){
		b.cards[b.nb_cards] = drawCard(b.deck);
		b.nb_cards++;
		for(i=0;i<nb_players;i++){
			//pthread_cond_wait(exdata[i]->cond);
			exdata[i].cards[exdata[i].nb_cards] = drawCard(b.deck);
			exdata[i].nb_cards++;
			//pthread_cond_signal(&exdata[i].cond);
		}
	}
	for(i=0;i<nb_players;i++){
		pthread_join(tid[i],NULL);
	}
	*/
	init_data(&exdata[0]);
	printf("nb_cards thp %d\n",exdata[0].nb_cards);
	pthread_create(&tid[0],NULL,player,&exdata[0]);
	pthread_mutex_lock(&exdata[0].mut);
	exdata[0].cards[0] = drawCard(b.deck);
	pthread_cond_signal(&exdata[0].cond);
	pthread_mutex_unlock(&exdata[0].mut);
	
	pthread_join(tid[0],NULL);
	//for(i=0;i<nb_players;i++){
	//	printf("%d\n",exdata[i].nb_cards);
	//}
	
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