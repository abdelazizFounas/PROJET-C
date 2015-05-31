#include <stdlib.h>
#include <stdio.h>

/* DECLARATION TYPES */

typedef struct arbre_n arbre_n;
struct arbre_n {
	unsigned int code;
	char* mot;
	struct arbre_n* freres;
	struct arbre_n* enfant;
};


/* DECLARATION FONCTIONS */

/* fonction de creation d'un element du dictionnaire */
arbre_n* create_elem(unsigned int code, char* mot);

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base */
void init();

/* fonction de destruction d'un dictionnaire */
void destruction();

/* fonction de recherche d'un code dans la liste */
arbre_n* search (char* string);

/* fonction auxiliaire de recherche d'un mot dans les freres d'un noeud en gardant l'ordre alphabetique */
arbre_n* arbre_mot_comp (char* string, arbre_n* ptr_tmp, unsigned int i);

/* fonction renvoyant un booleen pour dire si un mot existe dans le dico */
int exist (char* prefixe, char suffixe);

/*fonction renvoyant le code a partir d'une chaine de caractere */
unsigned int find_code (char* string);

/* fonction d'insertion d'un code dans la liste*/
void insert (char* prefixe, char suffixe);

/* fonction concatenant une string et un caractere */
char* cat_str_char (char* str, char car);

/* fonction affichant le dico */
void toStr();

/* fontion d'acces a une chaine depuis un code */
char* search_by_code(int code);
