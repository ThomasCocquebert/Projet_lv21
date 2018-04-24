#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "deck.h"
#include "joueur.h"

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t start_th = PTHREAD_COND_INITIALIZER;
pthread_cond_t fin_pioche = PTHREAD_COND_INITIALIZER;
pthread_cond_t fin_copie_pioche = PTHREAD_COND_INITIALIZER;
pthread_cond_t fin_carte_sup = PTHREAD_COND_INITIALIZER;
pthread_cond_t fin_newcarte = PTHREAD_COND_INITIALIZER;
pthread_cond_t transfert_gain = PTHREAD_COND_INITIALIZER;

PLAYER init_ex(PLAYER p){
	p.nb_cards = 0;
	p.tot_cards = 0;
	p.jetons = 1000;
	p.valStop = 18;
	p.obj_jetons = 2000;
	p.type_mise = 0;
	p.mise_base = 100;
	p.win = 0;
	p.br = 0;
	p.mise = 100;
	return p;
}

void copie_start_data(EXDATA *d,PLAYER p){
	d->nb_cards = p.nb_cards;
	d->tot_cards = p.tot_cards;
	d->jetons = p.jetons;
	d->valStop = p.valStop;
	d->win = p.win;
	if(p.type_mise == 0) d->mise = p.mise_base;
	if(p.type_mise == 1){
		if(p.win==1) d->mise = 2*p.mise;
		if(p.win==-1) d->mise = p.mise_base;
		if(p.win==0) d->mise = p.mise_base;
	}
	if(p.type_mise == 2){
		if(p.win==1){
			if(d->mise/2 == 0) d->mise = 1;
			else d->mise = p.mise/2;
		}
		if(p.win==-1) d->mise = p.mise_base;
		if(p.win==0) d->mise = p.mise_base;
	}
}

void pioche_initiale(BANK *b,EXDATA *d){
	int i;
	for(i=0;i<2;i++){
		b->cards[i] = drawCard(b->deck);
		b->nb_cards++;
		d->cards[i] = drawCard(b->deck);
		d->nb_cards++;
	}
}

void transfert_pioche_initiale(PLAYER *p, EXDATA *d){
	int i;
	printf("Cartes transférées : ");
	for(i=0;i<2;i++){
		p->cards[i] = d->cards[i];
		printCard(p->cards[i]);
		printf(" ");
	}
	printf("\n");
	p->nb_cards = d->nb_cards;
}

void total_card_data(EXDATA *d){
	int i;
	d->tot_cards = 0;
	for(i=0;i<d->nb_cards-1;i++){
		d->tot_cards += getValueFromCardID(d->cards[i]);
	}
}

void add_card_data(EXDATA *d,BANK *b){
	d->cards[d->nb_cards] = drawCard(b->deck);
	d->tot_cards += getValueFromCardID(d->cards[d->nb_cards]);
	d->nb_cards += 1;
}

void transfert_data_newcarte(EXDATA *d, PLAYER *p){
	p->tot_cards = d->tot_cards;
	int i;
	for(i=p->nb_cards;i<d->nb_cards-1;i++){
		p->cards[i] = d->cards[i];
	}
	p->nb_cards = d->nb_cards;
}

void pioche_carte_b(BANK *b){
	if(b->tot_cards<17){
		b->cards[b->nb_cards] = drawCard(b->deck);
		b->nb_cards++;
	}
}

void gain_j(BANK b, EXDATA *d){
	if(b.tot_cards<=21 && d->tot_cards<=21){
		if(b.tot_cards<d->tot_cards && d->tot_cards!=21){
			d->gain = 2*d->mise;
			d->win = 1;
		}
		if(b.tot_cards<d->tot_cards && d->tot_cards==21){
			d->gain = 3*d->mise;
			d->win = 1;
		}
		if(b.tot_cards==d->tot_cards){
			d->gain = d->mise;
			d->win = 1;
		}
		if(b.tot_cards>d->tot_cards){
			d->gain = 0;
			d->win = -1;
		}
	}
	if(b.tot_cards>21 && d->tot_cards>21) {
		d->gain = d->mise;
		d->win = 1;
	}
	if(b.tot_cards>21 && d->tot_cards<=21){
		if(d->tot_cards == 21){
			d->gain = 3*d->mise;
			d->win = 1;
		}
		if(d->tot_cards != 21){
			d->gain = 2*d->mise;
			d->win = 1;
		}
	}
	if(b.tot_cards<=21 && d->tot_cards>21){
		d->gain = 0;
		d->win = -1;
	}
	d->jetons += d->gain - d->mise;
}

void maj_gain(PLAYER *p,EXDATA *d){
	p->jetons = d->jetons;
	p->win = d->win;
}

void afficher_j(PLAYER p){
	int i;
	printf("_____JOUEUR_____\n");
	printf("nb_cards %d\n",p.nb_cards);
	for(i=0;i<p.nb_cards-1;i++){
		printCard(p.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("tot_cards %d\n",p.tot_cards);
	printf("jetons %d\n",p.jetons);
	printf("valStop %d\n",p.valStop);
	printf("obj_jetons %d\n",p.obj_jetons);
	printf("type_mise %d\n",p.type_mise);
	printf("mise_base %d\n",p.mise_base);
	printf("win %d\n",p.win);
	printf("br %d\n",p.br);
	printf("mise %d\n",p.mise);
	printf("________________\n");
}

void afficher_b(BANK b){
	int i;
	printf("_____BANK_____\n");
	printf("nb_cards %d\n",b.nb_cards);
	for(i=0;i<b.nb_cards-1;i++){
		printCard(b.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("tot_cards %d\n",b.tot_cards);
	printf("______________\n");
}

void afficher_d(EXDATA d){
	int i;
	printf("_____DATA_____\n");
	printf("> nb_cards %d\n",d.nb_cards);
	printf("==> ");
	for(i=0;i<d.nb_cards-1;i++){
		printCard(d.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("valStop %d\n",d.valStop);
	printf("==> tot_cards %d\n",d.tot_cards);
	printf("mise %d\n",d.mise);
	printf("gain %d\n",d.gain);
	printf("jetons %d\n",d.jetons);
	printf("win %d\n",d.win);
}

void *jouer(void *arg){
	EXDATA *d = arg;
	PLAYER p = init_ex(p);
	copie_start_data(d,p);
	
	pthread_mutex_lock(&mut);
	
	pthread_cond_signal(&start_th);
	pthread_cond_wait(&fin_pioche,&mut);
	{
		transfert_pioche_initiale(&p,d);
		pthread_cond_signal(&fin_copie_pioche);
	}
	pthread_cond_wait(&fin_carte_sup,&mut);
	{
		transfert_data_newcarte(d,&p);
		pthread_cond_signal(&fin_newcarte);
	}
	pthread_cond_wait(&transfert_gain,&mut);
	{
		maj_gain(&p,d);
	}
	pthread_mutex_unlock(&mut);
	
	afficher_j(p);
	
	return NULL;
}

int main(){
	srand(time(NULL));
	BANK b = init_bank(b,P52,1,1,10);
	EXDATA d = init_data(d);
	pthread_t joueur;
	
	pthread_create(&joueur,NULL,jouer,&d);
	
	pthread_mutex_lock(&mut);
	
	pthread_cond_wait(&start_th,&mut);
	{
		pioche_initiale(&b,&d);
		pthread_cond_signal(&fin_pioche);
	}
	pthread_cond_wait(&fin_copie_pioche,&mut);
	{
		total_card_data(&d);
		if(d.tot_cards<d.valStop){
			d.tot_cards += getValueFromCardID(d.cards[d.nb_cards]);
		}
		
		pthread_cond_signal(&fin_carte_sup);
	}
	pthread_cond_wait(&fin_newcarte,&mut);
	{
		pioche_carte_b(&b);
		gain_j(b,&d);
		pthread_cond_signal(&transfert_gain);
	}
	
	pthread_mutex_unlock(&mut);
	
	pthread_join(joueur,NULL);
	
	afficher_b(b);
	afficher_d(d);
	
	libere_bank(b);
	
	exit(0);
}

/*
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inmutex = PTHREAD_COND_INITIALIZER;
pthread_cond_t copiecard = PTHREAD_COND_INITIALIZER;
pthread_cond_t newcard = PTHREAD_COND_INITIALIZER;
pthread_cond_t exit_newcard = PTHREAD_COND_INITIALIZER;
pthread_cond_t gain = PTHREAD_COND_INITIALIZER;
pthread_cond_t copiegain = PTHREAD_COND_INITIALIZER;
pthread_cond_t inboucle = PTHREAD_COND_INITIALIZER;

void transfert_pioche_initiale(PLAYER *p, EXDATA *d){
	int i;
	for(i=0;i<2;i++){
		p->cards[i] = d->cards[i];
		printCard(p->cards[i]);
		printf("\n");
	}
	p->nb_cards = d->nb_cards;
}

void afficher_j(PLAYER p){
	printf("nb_cards %d\n",p.nb_cards);
	printf("tot_cards %d\n",p.tot_cards);
	printf("jetons %d\n",p.jetons);
	printf("valStop %d\n",p.valStop);
	printf("obj_jetons %d\n",p.obj_jetons);
	printf("type_mise %d\n",p.type_mise);
	printf("win %d\n",p.win);
	printf("br %d\n",p.br);
	printf("mise %d\n",p.mise);
}

void *jouer(void *arg){
	PLAYER p;
	EXDATA *d = arg;
	p = init_joueur(p);
	p.valStop = 18;
	p.mise = 50;
	p.jetons = 1000;
	
	pthread_mutex_lock(&mut);
	
	d->valStop = p.valStop;
	pthread_cond_signal(&inmutex);
	
	pthread_cond_wait(&copiecard,&mut);
	transfert_pioche_initiale(&p,d);
	p = total_carte(p);
	
	if(p.tot_cards<p.valStop){
		pthread_cond_wait(&inboucle,&mut);
		printf("thread attends signal newcard\n");
		//Threads dans la boucle et attends carte
		pthread_cond_signal(&newcard);
		//Tjreads attends une nouvelle carte
		pthread_cond_wait(&newcard,&mut);
		p.nb_cards = d->nb_cards;
		p.cards[p.nb_cards -1] = d->cards[p.nb_cards -1];
	}
	else
	{
		pthread_cond_wait(&inmutex,&mut);
		pthread_cond_signal(&exit_newcard);
	}
	pthread_cond_wait(&gain,&mut);
	p = total_carte(p);
	printf("Total Joueur : %d\n",p.tot_cards);
	d->tot_cards = p.tot_cards;
	d->valStop = p.valStop;
	d->mise = p.mise;
	p.jetons -= p.mise;
	d->nb_jetons = p.jetons;
	
	
	p.jetons += d->gain;
	afficher_j(p);
	
	//pthread_cond_signal(&copiegain);
	
	pthread_mutex_unlock(&mut);
	
	return NULL;
}

void pioche_initiale(BANK *b,EXDATA *d){
	int i;
	for(i=0;i<2;i++){
		b->cards[i] = drawCard(b->deck);
		b->nb_cards++;
		d->cards[i] = drawCard(b->deck);
		d->nb_cards++;
	}
}

void pioche_carte_j(EXDATA *d, BANK *b){
	d->cards[d->nb_cards] = drawCard(b->deck);
	d->nb_cards++;
}

void pioche_carte_b(BANK *b){
	if(b->tot_cards<17){
		b->cards[b->nb_cards] = drawCard(b->deck);
		b->nb_cards++;
	}
}

void gain_j(BANK b, EXDATA *d){
	if(b.tot_cards<=21 && d->tot_cards<=21){
		if(b.tot_cards<d->tot_cards && d->tot_cards!=21) d->gain = 2*d->mise;
		if(b.tot_cards<d->tot_cards && d->tot_cards==21) d->gain = 3*d->mise;
		if(b.tot_cards==d->tot_cards) d->gain = d->mise;
		if(b.tot_cards>d->tot_cards) d->gain = 0;
	}
	if(b.tot_cards>21 && d->tot_cards>21) d->gain = d->mise;
	if(b.tot_cards>21 && d->tot_cards<=21){
		if(d->tot_cards == 21) d->gain = 3*d->mise;
		if(d->tot_cards != 21) d->gain = 2*d->mise;
	}
	if(b.tot_cards<=21 && d->tot_cards>21) d->gain = 0;
	
	d->nb_jetons += d->gain;
}


int main(){
	srand(time(NULL));
	BANK b = init_bank(b,P52,1,1,10);
	EXDATA d;
	pthread_t joueur;
	
	d = init_data(d);
	
	pthread_create(&joueur,NULL,jouer,&d);
	
	pthread_mutex_lock(&mut);
	
	pthread_cond_wait(&inmutex,&mut);
	pioche_initiale(&b,&d);
	b = total_carte_bank(b);
	pthread_cond_signal(&copiecard);
	
	d = total_carte_data(d);
	printf("%d < %d en dehors de la boucle",d.tot_cards,d.valStop);
	if(d.tot_cards<d.valStop){
		printf("%d < %d\n",d.tot_cards,d.valStop);
		//Main attends que threads dans boucle
		pthread_cond_signal(&inboucle);
		pthread_cond_wait(&newcard,&mut);
		pioche_carte_j(&d,&b);
		//Main envoie carte piocher
		pthread_cond_signal(&newcard);
	}
	else
	{
		pthread_cond_signal(&inmutex);
		pthread_cond_wait(&exit_newcard,&mut);
	}
	
	pioche_carte_b(&b);
	gain_j(b,&d);
	
	//sig 5
	pthread_cond_signal(&gain);
	
	//pthread_cond_wait(&copiegain);
	
	pthread_mutex_unlock(&mut);
	
	pthread_join(joueur,NULL);
	
	printf("nb_cards %d\n",b.nb_cards);
	printf("Carte Bank :");
	printCard(b.cards[0]);
	printf(" ");
	printCard(b.cards[1]);
	printf("\n");
	b = total_carte_bank(b);
	printf("Total bank : %d\n",b.tot_cards);
	
	libere_bank(b);
	exit(0);
}
*/