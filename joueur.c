#include "joueur.h"
#include "deck.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

PLAYER init_joueur(PLAYER p1)
{
	//p1->cards = malloc (sizeof(int)*21);
	p1.nb_cards = 0;
	p1.tot_cards = 0;
	p1.jetons = 0;
	p1.valStop = 0;
	p1.obj_jetons = 0;
	p1.mise = 0;
	p1.type_mise = 0;
	p1.win = 1;
	p1.br = 1;
	return p1;
}

BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck,
	       int nb_hand)
{
	b.nb_players = nb_players;
	b.deck = (initDeck(type, nb_deck));
	b.nb_hand = nb_hand;
	//b.cards = malloc (sizeof(int)*17);
	b.nb_cards = 0;
	b.tot_cards = 0;
	shuffleDeck(b.deck);
	return b;
}

int conversion_carte(int idcard)
{
	int val_card_int;
	cardvalue_t val_card = getValueFromCardID(idcard);
	switch (val_card) {
	case VA:
		return val_card_int = 11;
	case V2:
		return val_card_int = 2;
	case V3:
		return val_card_int = 3;
	case V4:
		return val_card_int = 4;
	case V5:
		return val_card_int = 5;
	case V6:
		return val_card_int = 6;
	case V7:
		return val_card_int = 7;
	case V8:
		return val_card_int = 8;
	case V9:
		return val_card_int = 9;
	case VX:
		return val_card_int = 10;
	case VJ:
		return val_card_int = 10;
	case VQ:
		return val_card_int = 10;
	case VK:
		return val_card_int = 10;
	case VJOK:
		return val_card_int = 10;
	default:
		return val_card_int = -1;
	}
}

PLAYER total_carte(PLAYER p1)
{
	int cpt;
	p1.tot_cards = 0;
	for (cpt = 0; cpt < p1.nb_cards; cpt++) {
		p1.tot_cards += conversion_carte(p1.cards[cpt]);
	}
	return p1;
}

BANK total_carte_bank(BANK b)
{
	int cpt;
	b.tot_cards = 0;
	for (cpt = 0; cpt < b.nb_cards; cpt++) {
		b.tot_cards += conversion_carte(b.cards[cpt]);
	}
	return b;
}

void libere_joueur(PLAYER * p)
{
	free(p->cards);
	free(p);
}

void libere_bank(BANK b)
{
	removeDeck(b.deck);
	//free(b.cards);
}

void libere_mem(int nb_players, PLAYER * p[], pthread_t tid[], BANK b)
{
	int i;
	for (i = 0; i < nb_players; i++) {
		libere_joueur(p[i]);
	}
	libere_bank(b);
	free(p);
	free(tid);
}

EXDATA init_data(EXDATA exdata )
{
	exdata.nb_cards = 0;
	exdata.mise = 0;
	exdata.gain = 0;
	exdata.jetons = 0;
	exdata.valStop = 0;
	exdata.tot_cards = 0;
	exdata.win = 0;
	exdata.maxhand = 0;
	exdata.br = 0;
	exdata.obj_jetons = 0;
	exdata.init_hand = 0;
	//exdata.mutcond = mutcond;
	return exdata;
}

EXDATA total_carte_data(EXDATA exdata)
{
	int cpt;
	for (cpt = 0; cpt < exdata.nb_cards; cpt++) {
		exdata.tot_cards += conversion_carte(exdata.cards[cpt]);
	}
	return exdata;
}

PLAYER init_ex(PLAYER p)
{
	p.nb_cards = 0;
	p.tot_cards = 0;
	p.jetons = 10000;
	p.valStop = 18;
	p.obj_jetons = 20000;
	p.type_mise = 0;
	p.mise_base = 100;
	p.win = 0;
	p.br = 0;
	p.mise = 100;
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
		printf("%d\n",b->cards[i]);
		if(b->cards[i] == -1){
			removeDeck(b->deck);
			b->deck = initDeck(P52,1);
			shuffleDeck(b->deck);
			b->cards[i] = drawCard(b->deck);
			printf("shuffle %d\n",b->cards[i]);
		}
		b->nb_cards++;
		d->cards[i] = drawCard(b->deck);
		printf("%d\n",d->cards[i]);
		if(d->cards[i] == -1){
			removeDeck(b->deck);
			b->deck = initDeck(P52,1);
			shuffleDeck(b->deck);
			d->cards[i] = drawCard(b->deck);
			printf("shuffle %d\n",d->cards[i]);
		}
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
	if(d->cards[d->nb_cards] == -1){
			shuffleDeck(b->deck);
			d->cards[d->nb_cards] = drawCard(b->deck);
		}
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
		printf("%d\n",b.cards[b.nb_cards]);
		if(b.cards[b.nb_cards] == -1){
			removeDeck(b.deck);
			b.deck = initDeck(P52,1);
			shuffleDeck(b.deck);
			b.cards[b.nb_cards] = drawCard(b.deck);
			printf("%d\n",b.cards[b.nb_cards]);
		}
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