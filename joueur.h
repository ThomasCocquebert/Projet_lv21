#include "deck.h"
#include <pthread.h>

/*
La structure accueillant les informations propres aux joueurs
int cards renvoie vers un tableau de 21, puisque qu'un joueur ne peut faire plus de 21 le pire cas serai de piocher 21 As valant 1
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
	int nb_deck;
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
	int type_mise;
	int mise_base;
} EXDATA;

//Initialise le joueur
PLAYER init_joueur(PLAYER p1);

//Initialise la banque
BANK init_bank(BANK b, decktype_t type, int nb_players, int nb_deck, int nb_hand);

//Initialise data
EXDATA init_data(EXDATA d, int valStop, int jetons, int obj_jetons, int type_mise, int mise_base);

//Distribution des cartes entre la banque et le joueur
void pioche_initiale(BANK * b, EXDATA * d);

//Copie les cartes et le nombre de cartes de EXDATA vers PLAYER
void transfert_pioche_initiale(PLAYER * p, EXDATA * d);

//Définie la mise des joueurs
PLAYER mise_joueur(PLAYER p, EXDATA * d);

//Convertie une idcard en valeur numérique qui sera utile pour le total des cartes du joueur
int conversion_carte(int idcard);

//Calcul le total des carte dans la structure EXDATA
EXDATA total_carte_data(EXDATA exdata);

//Transfert les nouvelles cartes vers PLAYER
void transfert_data_newcarte(EXDATA * d, PLAYER * p);

//Additione la valeur de chaque carte que possède le joueur dans le tableau de ses cartes
PLAYER total_carte(PLAYER p1);

//Pioche de carte suppélmentaire pour la banque
BANK pioche_carte_b(BANK b);

//Additione la valeur de chaque carte que possède la banque dans le tableau de ses cartes
BANK total_carte_bank(BANK b);

//Calcul les gains de EXDATA
void gain_j(BANK b, EXDATA * d);

//Affiche un message avec les mêmes infos que le fichiers de sortie
void afficher_msg(EXDATA d, BANK b);

//Supprime les infos de la banque qui ne serviront plus
BANK clean_b(BANK b);

//Test si on atteind le nombre de main maximales
int test_maxhand(int cpthand, int hand);

//Mets à jour les informations de gain de EXDATAvers PLAYER
void maj_gain(PLAYER * p, EXDATA * d);

//Supprime les informations du joueur qui ne serviront plus
PLAYER clean_j(PLAYER p);

//Supprime les infos d'EXDATA qui ne serviront plus
void clean_d(EXDATA * d);

//Libere la mémoire du tableau de carte et du deck
void libere_bank(BANK b);

//Le reste des fonction présentes n'ont pas été implémenté dans le main mais ont été utilisé auparavant pour des test
//et autres essaies. Ils sont séparés des fonctions vraiment utiles à ce programme

//libere la mémoire du tableau cards
void libere_joueur(PLAYER *p);

//Libere la mémoire de la bank et des joueurs
void libere_mem(int nb_players, PLAYER *p[], pthread_t tid[], BANK b);

//Ancienne version d'initialisation
EXDATA init_data_old(EXDATA exdata);

//Copie les valeurs de EXDATA dans PLAYER
void copie_start_data(EXDATA * d, PLAYER * p);

//Calcul le total des cartes de EXDATA
EXDATA total_card_data(EXDATA d);

//Ajoute une carte à EXDATA
void add_card_data(EXDATA * d, BANK * b);

//Affiche les infos d'un joueur
void afficher_j(PLAYER p);

//Affiche les infos de la banque
void afficher_b(BANK b);

//Affiche lesinfos d'EXDATA
void afficher_d(EXDATA d);

//AFFiche les cartes d'EXDATA
void afficher_carte_d(EXDATA d);

//Affiche les carte de la banque
void afficher_carte_b(BANK b);