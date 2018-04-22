#include "joueur.h"
#include "deck.h"

#include <stdlib.h>
#include <stdio.h>

PLAYER *init_joueur(PLAYER *p1){
	p1->cards = malloc (sizeof(int)*21);
	p1->nb_cards = 0;
	p1->tot_cards = 0;
	p1->jetons = 0;
	p1->valStop = 0;
	p1->obj_jetons = 0;
	p1->type_mise = 0;
	return p1;
}

BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck, int nb_hand){
	b.nb_players = nb_players;
	b.deck = (initDeck(type,nb_deck));
	b.nb_hand = nb_hand;
	b.cards = malloc (sizeof(int)*17);
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
}

void libere_bank(BANK b){
	removeDeck(b.deck);
	free(b.cards);
}