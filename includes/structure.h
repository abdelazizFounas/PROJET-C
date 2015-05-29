/* Fichier contenant les structures de donnees utilisees 
et les en-tete de fonction les manipulant 
cree le 28/05 */


#include <stdlib.h>
#include <stdio.h>


/* Variables globales */




/* DECLARATION DES STRUCTURES */

/* STRUCTURE dict_elem : liste des codes */
typedef struct dict_elem dict_elem;
struct dict_elem {
	unsigned int code;
	char* mot;
	struct dict_elem* next;
};

/* STRUCTURE ... */

/*
création d'un tableau de code pour aller plus vite ?
--> chaine inutile apres, ou uniquement pour codes suivants les 256 premiers basiques

penser a rajouter un pointeur sur le dernier element de la liste !!!!!!
*/

/* DECLARATION DES FONCTIONS */

/* fonction de creation d'un element du dictionnaire */
dict_elem* create_elem(unsigned int code, char* mot);

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base */
void init();

void destruction();



/* fonction longueur de chaine */
unsigned int length (dict_elem* ptr_dict_elem);

/* fonction de recherche d'un code dans la liste */
dict_elem* search (char* string);

int exist (char* prefixe, char suffixe);

unsigned int find_code (char* string);

/* fonction d'insertion d'un code dans la liste*/
void insert (char* prefixe, char suffixe);

char* cat_str_char (char* str, char car);

void toStr();