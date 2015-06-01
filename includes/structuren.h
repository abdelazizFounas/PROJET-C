#include <stdlib.h>
#include <stdio.h>

/* DECLARATION TYPES */
typedef struct chaine_octet chaine_octet;
struct chaine_octet{
	unsigned int octet;
	chaine_octet* next;
};

typedef struct arbre_n arbre_n;
struct arbre_n {
	unsigned int code;
	unsigned int code_prefixe;
	chaine_octet* mot;
	struct arbre_n* freres;
	struct arbre_n* enfant;
};



/* DECLARATION FONCTIONS */

/* fonction de creation d'un element du dictionnaire */
arbre_n* create_elem(unsigned int code, chaine_octet* chaine, unsigned int code_pref);

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base */
arbre_n* init();

/* fonction de destruction d'un dictionnaire */
void destruction();

/* fonction de recherche d'un code dans la liste */
arbre_n* search (chaine_octet* chaine);

/* fonction auxiliaire de recherche d'un mot dans les freres d'un noeud en gardant l'ordre alphabetique */
arbre_n* arbre_mot_comp (chaine_octet* chaine1, arbre_n* ptr_tmp, unsigned int i);

/* fonction renvoyant un booleen pour dire si un mot existe dans le dico */
int exist (chaine_octet* prefixe, unsigned int suffixe);

int nb_octets(chaine_octet* chaine);

/*fonction renvoyant le code a partir d'une chaine de caractere */
unsigned int find_code (chaine_octet* chaine);

/* fonction d'insertion d'un code dans la liste*/
void insert (chaine_octet* chaine, unsigned int suffixe);

void effacer_octet(chaine_octet* tete);

/* fonction concatenant une string et un caractere */
chaine_octet* cat_str_char (chaine_octet* str, unsigned int car);

/* fonction affichant le dico */
//void toStr();

//void toStrP(arbre_n* ptr, int i);

/* fontion d'acces a une chaine depuis un code */
chaine_octet* search_by_code(int code);

void supp_dern_elem(chaine_octet* chaine);

void afficher_octet(chaine_octet* chaine);

int cmp_chaine_octet(chaine_octet* chaine1, chaine_octet* chaine2);

int cmp_chaine_octet_n(chaine_octet* chaine1, chaine_octet* chaine2, int n);

void chaine_cat(chaine_octet* chaine1, chaine_octet* chaine2);

chaine_octet* chaine_cpy(chaine_octet* str);