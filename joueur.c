#include "joueur.h"
#include "deck.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck,
	       int nb_hand)
{
	b.nb_players = nb_players;
	b.deck = (initDeck(type, nb_deck));
	b.nb_hand = nb_hand;
	//b.cards = malloc (sizeof(int)*17);
	b.nb_cards = 0;
	b.tot_cards = 0;
	b.nb_deck = nb_deck;
	shuffleDeck(b.deck);
	return b;
}

PLAYER init_joueur(PLAYER p)
{
	p.nb_cards = 0;
	p.tot_cards = 0;
	p.jetons = 0;
	p.valStop = 0;
	p.obj_jetons = 0;
	p.type_mise = 0;
	p.mise_base = 0;
	p.win = 0;
	p.br = 0;
	p.mise = 0;
	return p;
}

EXDATA init_data(EXDATA d, int valStop, int jetons, int obj_jetons, int type_mise, int mise_base){
	d.nb_cards = 0;
	d.valStop = valStop;
	d.mise = 0;
	d.gain = 0;
	d.jetons = jetons;
	d.tot_cards = 0;
	d.win = 0;
	d.maxhand = 0;
	d.br = 0;
	d.obj_jetons = obj_jetons;
	d.type_mise = type_mise;
	d.mise_base = mise_base;
	return d;
}

void pioche_initiale(BANK * b, EXDATA * d)
{
	int i;
	for (i = 0; i < 2; i++) {
		b->cards[i] = drawCard(b->deck);
		if(b->cards[i] == -1){
			removeDeck(b->deck);
			b->deck = initDeck(P52,b->nb_deck);
			shuffleDeck(b->deck);
			b->cards[i] = drawCard(b->deck);
		}
		b->nb_cards++;
		d->cards[i] = drawCard(b->deck);
		if(d->cards[i] == -1){
			removeDeck(b->deck);
			b->deck = initDeck(P52,b->nb_deck);
			shuffleDeck(b->deck);
			d->cards[i] = drawCard(b->deck);
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

PLAYER mise_joueur(PLAYER p, EXDATA * d){
	if(p.win == 0) p.mise = p.mise_base;
	if(p.win == -1){
		if(p.type_mise == 0) p.mise = p.mise_base;
		if(p.type_mise == 1) p.mise = 2*p.mise;
		if(p.type_mise == 2){
			if(p.mise/2 == 0) p.mise = 1;
			else p.mise = p.mise/2;
		}
	}
	if(p.win == 1) p.mise = p.mise_base;
	d->mise = p.mise;
	return p;
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

EXDATA total_card_data(EXDATA d)
{
	int cpt;
	d.tot_cards = 0;
	for (cpt = 0; cpt < d.nb_cards; cpt++) {
		d.tot_cards += conversion_carte(d.cards[cpt]);
	}
	return d;
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

PLAYER total_carte(PLAYER p1)
{
	int cpt;
	p1.tot_cards = 0;
	for (cpt = 0; cpt < p1.nb_cards; cpt++) {
		p1.tot_cards += conversion_carte(p1.cards[cpt]);
	}
	return p1;
}

BANK pioche_carte_b(BANK b)
{
	b = total_carte_bank(b);
	while (b.tot_cards < 17) {
		b.cards[b.nb_cards] = drawCard(b.deck);
		if(b.cards[b.nb_cards] == -1){
			removeDeck(b.deck);
			b.deck = initDeck(P52,b.nb_deck);
			shuffleDeck(b.deck);
			b.cards[b.nb_cards] = drawCard(b.deck);
		}
		b.nb_cards++;
		b = total_carte_bank(b);
	}
	return b;
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

void maj_gain(PLAYER * p, EXDATA * d)
{
	p->jetons = d->jetons;
	p->win = d->win;
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

void libere_bank(BANK b)
{
	removeDeck(b.deck);
}

//
//

void libere_joueur(PLAYER * p)
{
	free(p->cards);
	free(p);
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

EXDATA init_data_old(EXDATA exdata )
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
	exdata.type_mise = 0;
	exdata.mise_base = 0;
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

void copie_start_data(EXDATA * d, PLAYER * p)
{
	p->nb_cards = d->nb_cards;
	p->tot_cards = d->tot_cards;
	p->jetons = d->jetons;
	p->valStop = d->valStop;
	p->obj_jetons = d->obj_jetons;
	p->type_mise = d->type_mise;
	p->mise_base = d->mise_base;
	p->win= d->win;
	p->br = d->br;
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