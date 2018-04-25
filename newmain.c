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
pthread_cond_t boucle_hand = PTHREAD_COND_INITIALIZER;

PLAYER init_ex(PLAYER p)
{
	p.nb_cards = 0;
	p.tot_cards = 0;
	p.jetons = 1000;
	p.valStop = 18;
	p.obj_jetons = 2000;
	p.type_mise = 0;
	p.mise_base = 1000;
	p.win = 0;
	p.br = 0;
	p.mise = 1000;
	return p;
}

void copie_start_data(EXDATA * d, PLAYER p)
{
	d->nb_cards = p.nb_cards;
	d->tot_cards = p.tot_cards;
	d->jetons = p.jetons;
	d->valStop = p.valStop;
	d->win = p.win;
	if (p.type_mise == 0)
		d->mise = p.mise_base;
	if (p.type_mise == 1) {
		if (p.win == 1)
			d->mise = 2 * p.mise;
		if (p.win == -1)
			d->mise = p.mise_base;
		if (p.win == 0)
			d->mise = p.mise_base;
	}
	if (p.type_mise == 2) {
		if (p.win == 1) {
			if (d->mise / 2 == 0)
				d->mise = 1;
			else
				d->mise = p.mise / 2;
		}
		if (p.win == -1)
			d->mise = p.mise_base;
		if (p.win == 0)
			d->mise = p.mise_base;
	}
}

void pioche_initiale(BANK * b, EXDATA * d)
{
	int i;
	for (i = 0; i < 2; i++) {
		b->cards[i] = drawCard(b->deck);
		b->nb_cards++;
		d->cards[i] = drawCard(b->deck);
		d->nb_cards++;
	}
}

void transfert_pioche_initiale(PLAYER * p, EXDATA * d)
{
	int i;
	for (i = 0; i < 2; i++) {
		p->cards[i] = d->cards[i];
	}
	p->nb_cards = d->nb_cards;
}

void total_card_data(EXDATA * d)
{
	int i;
	d->tot_cards = 0;
	for (i = 0; i < d->nb_cards; i++) {
		d->tot_cards += getValueFromCardID(d->cards[i]);
	}
}

void add_card_data(EXDATA * d, BANK * b)
{
	d->cards[d->nb_cards] = drawCard(b->deck);
	d->tot_cards += getValueFromCardID(d->cards[d->nb_cards]);
	d->nb_cards += 1;
}

void transfert_data_newcarte(EXDATA * d, PLAYER * p)
{
	p->tot_cards = d->tot_cards;
	int i;
	for (i = p->nb_cards; i < d->nb_cards; i++) {
		p->cards[i] = d->cards[i];
	}
	p->nb_cards = d->nb_cards;
}

BANK pioche_carte_b(BANK b)
{
	b = total_carte_bank(b);
	if (b.tot_cards < 17) {
		b.cards[b.nb_cards] = drawCard(b.deck);
		b.nb_cards++;
		b = total_carte_bank(b);
	}
	return b;
}

void gain_j(BANK b, EXDATA * d)
{
	if (b.tot_cards <= 21 && d->tot_cards <= 21) {
		if (b.tot_cards < d->tot_cards && d->tot_cards != 21) {
			d->gain = 2 * d->mise;
			d->win = 1;
		}
		if (b.tot_cards < d->tot_cards && d->tot_cards == 21) {
			d->gain = 3 * d->mise;
			d->win = 1;
		}
		if (b.tot_cards == d->tot_cards) {
			d->gain = d->mise;
			d->win = 1;
		}
		if (b.tot_cards > d->tot_cards) {
			d->gain = 0;
			d->win = -1;
		}
	}
	if (b.tot_cards > 21 && d->tot_cards > 21) {
		d->gain = d->mise;
		d->win = 1;
	}
	if (b.tot_cards > 21 && d->tot_cards <= 21) {
		if (d->tot_cards == 21) {
			d->gain = 3 * d->mise;
			d->win = 1;
		}
		if (d->tot_cards != 21) {
			d->gain = 2 * d->mise;
			d->win = 1;
		}
	}
	if (b.tot_cards <= 21 && d->tot_cards > 21) {
		d->gain = 0;
		d->win = -1;
	}
	d->jetons += d->gain - d->mise;
}

void maj_gain(PLAYER * p, EXDATA * d)
{
	p->jetons = d->jetons;
	p->win = d->win;
}

void afficher_j(PLAYER p)
{
	int i;
	printf("_____JOUEUR_____\n");
	printf("nb_cards %d\n", p.nb_cards);
	for (i = 0; i < p.nb_cards; i++) {
		printCard(p.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("tot_cards %d\n", p.tot_cards);
	printf("jetons %d\n", p.jetons);
	printf("valStop %d\n", p.valStop);
	printf("obj_jetons %d\n", p.obj_jetons);
	printf("type_mise %d\n", p.type_mise);
	printf("mise_base %d\n", p.mise_base);
	printf("win %d\n", p.win);
	printf("br %d\n", p.br);
	printf("mise %d\n", p.mise);
	printf("________________\n\n");
}

void afficher_b(BANK b)
{
	int i;
	printf("_____BANK_____\n");
	printf("nb_cards %d\n", b.nb_cards);
	for (i = 0; i < b.nb_cards; i++) {
		printCard(b.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("tot_cards %d\n", b.tot_cards);
	printf("______________\n\n");
}

void afficher_d(EXDATA d)
{
	int i;
	printf("_____DATA_____\n");
	printf("> nb_cards %d\n", d.nb_cards);
	printf("==> ");
	for (i = 0; i < d.nb_cards; i++) {
		printCard(d.cards[i]);
		printf(" ");
	}
	printf("\n");
	printf("valStop %d\n", d.valStop);
	printf("==> tot_cards %d\n", d.tot_cards);
	printf("mise %d\n", d.mise);
	printf("gain %d\n", d.gain);
	printf("jetons %d\n", d.jetons);
	printf("win %d\n", d.win);
	printf("______________\n\n");
}

void afficher_carte_d(EXDATA d)
{
	int i;
	for (i = 0; i < d.nb_cards; i++) {
		printCard(d.cards[i]);
	}
	printf(" ; ");
}

void afficher_carte_b(BANK b)
{
	int i;
	for (i = 0; i < b.nb_cards; i++) {
		printCard(b.cards[i]);
	}
	printf(" ; ");
}

void afficher_msg(EXDATA d, BANK b)
{
	printf("_____MSG_____\n");
	afficher_carte_d(d);
	printf("%d ; ", d.tot_cards);
	afficher_carte_b(b);
	printf("%d ; ", b.tot_cards);
	printf("%d ; ", d.mise);
	printf("%d ; ", d.gain);
	printf("%d\n", d.jetons);
	printf("_____________\n");
}

PLAYER clean_j(PLAYER p)
{
	p.tot_cards = 0;
	p.nb_cards = 0;
	return p;
}

void clean_d(EXDATA * d)
{
	d->nb_cards = 0;
	d->tot_cards = 0;
}

BANK clean_b(BANK b)
{
	b.nb_cards = 0;
	b.tot_cards = 0;
	return b;
}

int test_maxhand(int cpthand, int hand)
{
	if (cpthand == hand)
		return 1;
	return 0;
}

void *jouer(void *arg)
{
	EXDATA *d = arg;
	PLAYER p = init_ex(p);
	copie_start_data(d, p);

	while (d->maxhand == 0 && d->br == 0 && (d->jetons < d->obj_jetons)) {
		pthread_mutex_lock(&mut);

		pthread_cond_signal(&start_th);
		pthread_cond_wait(&fin_pioche, &mut);
		{
			transfert_pioche_initiale(&p, d);
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
		//pthread_cond_wait(&boucle_hand, &mut);
		pthread_mutex_unlock(&mut);
	}

	return NULL;
}

int main()
{
	srand(time(NULL));
	BANK b = init_bank(b, P52, 1, 10, 10);
	EXDATA d = init_data(d);
	d.jetons = 1000;
	d.obj_jetons = 2000;
	pthread_t joueur;
	int cpthand = 0;

	pthread_create(&joueur, NULL, jouer, &d);

	while (d.maxhand == 0 && d.br == 0 && (d.jetons < d.obj_jetons)) {
		pthread_mutex_lock(&mut);
		
		pthread_cond_signal(&boucle_hand);
		pthread_cond_wait(&start_th, &mut);
		{
			pioche_initiale(&b, &d);
			pthread_cond_signal(&fin_pioche);
		}
		pthread_cond_wait(&fin_copie_pioche, &mut);
		{
			total_card_data(&d);
			if (d.tot_cards < d.valStop) {
				d.cards[d.nb_cards] = drawCard(b.deck);
				d.nb_cards++;
				total_card_data(&d);
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
