#include "deck.h"
#include <pthread.h>

/*
La structure accueillant les informations propres aux joueurs
int *cards renvoie vers un pointeur qui sera un tableau de 21, puisque qu'un joueur ne peut faire plus de 21 le pire cas serai de piocher 21 As valant 1
int type_mise sera une indication sur le comportement du joueur
*/
typedef struct player {
	int cards[21];
	int nb_cards;
	int tot_cards;
	int jetons;
	int valStop;
	int obj_jetons;
	int type_mise;
	int mise_base;
	int win;
	int br;
	int mise;
} PLAYER;

typedef struct banque{
	int nb_players;
	deck_t *deck;
	int nb_hand;
	int cards[17];
	int nb_cards;
	int tot_cards;
} BANK;

typedef struct exchange_data{ 
	int nb_cards;
	int cards[21];
	int valStop;
	int mise;
	int gain;
	int jetons;
	int tot_cards;
	int win;
	int maxhand;
	int br;
	int obj_jetons;
	int init_hand;
} EXDATA;

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
void libere_joueur(PLAYER *p);

//Libere la mémoire du tableau de carte et du deck
void libere_bank(BANK b);

//Libere la mémoire de la bank et des joueurs
void libere_mem(int nb_players, PLAYER *p[], pthread_t tid[], BANK b);

//Init data
EXDATA init_data(EXDATA exdata);

EXDATA total_carte_data(EXDATA exdata);

PLAYER init_ex(PLAYER p);

void copie_start_data(EXDATA * d, PLAYER p);

void pioche_initiale(BANK * b, EXDATA * d);

void transfert_pioche_initiale(PLAYER * p, EXDATA * d);

void total_card_data(EXDATA * d);

void add_card_data(EXDATA * d, BANK * b);

void transfert_data_newcarte(EXDATA * d, PLAYER * p);

BANK pioche_carte_b(BANK b);

void gain_j(BANK b, EXDATA * d);

void maj_gain(PLAYER * p, EXDATA * d);

void afficher_j(PLAYER p);

void afficher_b(BANK b);

void afficher_d(EXDATA d);

void afficher_carte_d(EXDATA d);

void afficher_carte_b(BANK b);

void afficher_msg(EXDATA d, BANK b);

PLAYER clean_j(PLAYER p);

void clean_d(EXDATA * d);

BANK clean_b(BANK b);

int test_maxhand(int cpthand, int hand);