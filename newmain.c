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

void *jouer(void *arg)
{
	EXDATA *d = arg;
	PLAYER p = init_joueur(p);
	copie_start_data(d,&p);

	while (d->maxhand == 0 && d->br == 0 && (d->jetons < d->obj_jetons)) {
		pthread_mutex_lock(&mut);
		
		pthread_cond_signal(&start_th);
		pthread_cond_wait(&fin_pioche, &mut);
		{
			transfert_pioche_initiale(&p, d);
			p = mise_joueur(p,d);
			pthread_cond_signal(&fin_copie_pioche);
		}
		pthread_cond_wait(&fin_carte_sup, &mut);
		{
			transfert_data_newcarte(d, &p);
			p = total_carte(p);
			d->tot_cards = p.tot_cards;
			pthread_cond_signal(&fin_newcarte);
		}
		pthread_cond_wait(&transfert_gain, &mut);
		{
			maj_gain(&p, d);
			p = clean_j(p);
			clean_d(d);
		}
		pthread_mutex_unlock(&mut);
	}

	return NULL;
}

int main()
{
	srand(time(NULL));
	int valStop = 18;
	int jetons = 1000;
	int obj_jetons = 2000;
	int type_mise = 0;
	int mise_base = 100;
	int nb_deck = 1;
	int nb_players = 1;
	int nb_hand = 100;
	BANK b = init_bank(b, P52, nb_players, nb_deck, nb_hand);
	EXDATA d = init_data(d,valStop,jetons,obj_jetons,type_mise,mise_base);
	pthread_t joueur;
	int cpthand = 0;

	pthread_create(&joueur, NULL, jouer, &d);

	while (d.maxhand == 0 && d.br == 0 && (d.jetons < d.obj_jetons)) {
		pthread_mutex_lock(&mut);
		
		pthread_cond_wait(&start_th, &mut);
		{
			printf("Tour %d\n",cpthand);
			pioche_initiale(&b, &d);
			pthread_cond_signal(&fin_pioche);
		}
		pthread_cond_wait(&fin_copie_pioche, &mut);
		{
			d = total_card_data(d);
			while (d.tot_cards < d.valStop) {
				d.cards[d.nb_cards] = drawCard(b.deck);
				if(d.cards[d.nb_cards] == -1){
					removeDeck(b.deck);
					b.deck = initDeck(P52,b.nb_deck);
					shuffleDeck(b.deck);
					d.cards[d.nb_cards] = drawCard(b.deck);
				}
				d.nb_cards++;
				d = total_card_data(d);
			}

			pthread_cond_signal(&fin_carte_sup);
		}
		pthread_cond_wait(&fin_newcarte, &mut);
		{
			b = pioche_carte_b(b);
			b = total_carte_bank(b);
			gain_j(b, &d);
			afficher_msg(d, b);
			b = clean_b(b);
			cpthand++;
			d.maxhand = test_maxhand(cpthand, b.nb_hand);
			if(d.jetons<=0) d.br = 1;
			if(d.jetons>=d.obj_jetons) d.br = 1;
			pthread_cond_signal(&transfert_gain);
		}

		pthread_mutex_unlock(&mut);
	}

	pthread_join(joueur, NULL);

	libere_bank(b);

	exit(0);
}
