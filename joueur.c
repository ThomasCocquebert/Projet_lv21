#include "joueur.h"
#include "deck.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

PLAYER init_joueur(PLAYER p1){
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

BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck, int nb_hand){
	b.nb_players = nb_players;
	b.deck = (initDeck(type,nb_deck));
	b.nb_hand = nb_hand;
	//b.cards = malloc (sizeof(int)*17);
	b.nb_cards = 0;
	b.tot_cards = 0;
	shuffleDeck(b.deck);
	return b;
}

int conversion_carte(int idcard){
	int val_card_int;
	cardvalue_t val_card = getValueFromCardID(idcard);
	switch(val_card) {
		case VA: return val_card_int = 11;
		case V2: return val_card_int = 2;
		case V3: return val_card_int = 3;
		case V4: return val_card_int = 4;
		case V5: return val_card_int = 5;
		case V6: return val_card_int = 6;
		case V7: return val_card_int = 7;
		case V8: return val_card_int = 8;
		case V9: return val_card_int = 9;
		case VX: return val_card_int = 10;
		case VJ: return val_card_int = 10;
		case VQ: return val_card_int = 10;
		case VK: return val_card_int = 10;
		case VJOK: return val_card_int = 10;
		default: return val_card_int = -1;
	}
}

PLAYER total_carte(PLAYER p1){
	int cpt;
	for(cpt=0;cpt<p1.nb_cards;cpt++){
		p1.tot_cards += conversion_carte(p1.cards[cpt]);
	}
	return p1;
}

BANK total_carte_bank(BANK b){
	int cpt;
	for(cpt=0;cpt<b.nb_cards;cpt++){
		b.tot_cards += conversion_carte(b.cards[cpt]);
	}
	return b;
}

void libere_joueur(PLAYER *p){
	free(p->cards);
	free(p);
}

void libere_bank(BANK b){
	removeDeck(b.deck);
	//free(b.cards);
}

void libere_mem(int nb_players, PLAYER *p[], pthread_t tid[], BANK b){
	int i;
	for(i=0;i<nb_players;i++){
		libere_joueur(p[i]);
	}
	libere_bank(b);
	free(p);
	free(tid);
}

EXDATA init_data(EXDATA exdata/*, TMC *mutcond*/){
	exdata.nb_cards = 0;
	exdata.mise = 0;
	exdata.gain = 0;
	exdata.jetons = 0;
	exdata.valStop = 0;
	exdata.tot_cards = 0;
	exdata.win = 0;
	//exdata.mutcond = mutcond;
	return exdata;
}

EXDATA total_carte_data(EXDATA exdata){
	int cpt;
	for(cpt=0;cpt<exdata.nb_cards;cpt++){
		exdata.tot_cards += conversion_carte(exdata.cards[cpt]);
	}
	return exdata;
}