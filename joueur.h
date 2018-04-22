#include "deck.h"

/*
La structure accueillant les informations propres aux joueurs
int *cards renvoie vers un pointeur qui sera un tableau de 21, puisque qu'un joueur ne peut faire plus de 21 le pire cas serai de piocher 21 As valant 1
int type_mise sera une indication sur le comportement du joueur
*/
typedef struct player {
	int *cards;
	int nb_cards;
	int tot_cards;
	int jetons;
	int valStop;
	int obj_jetons;
	int type_mise;
} PLAYER;

typedef struct banque{
	int nb_players;
	deck_t *deck;
	int nb_hand;
	int *cards;
	int nb_cards;
	int tot_cards;
} BANK;

//Initialise le joueur
PLAYER init_joueur(PLAYER p1);

//Initialise la banque
BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck, int nb_hand);

//Convertie une idcard en valeur numérique qui sera utile pour le total des cartes du joueur
int conversion_carte(int idcard);

//Additione la valeur de chaque carte que possède le joueur dans le tableau de ses cartes
PLAYER total_carte(PLAYER p1);

//Additione la valeur de chaque carte que possède la banque dans le tableau de ses cartes
BANK total_carte_bank(BANK b);

//libere la mémoire du tableau cards
void libere_joueur(PLAYER p);

//Libere la mémoire du tableau de carte et du deck
void libere_bank(BANK b);