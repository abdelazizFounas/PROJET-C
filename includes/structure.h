/* Fichier contenant les structures de donnees utilisees 
et les en-tete de fonction les manipulant 
cree le 28/05 */

#include <stdlib.h>
#include <stdio.h>

/* DECLARATION DES STRUCTURES */

/* STRUCTURE dict_elem : liste des codes et des mots */
typedef struct dict_elem dict_elem;
struct dict_elem {
	unsigned int code;
	char* mot;
	dict_elem* next;
};

/* DECLARATION DES PROTOTYPES */

/* fonction de creation d'un element du dictionnaire */
dict_elem* create_elem(unsigned int code, char* mot);

/* fonction d'initialisation de notre chaine avec les 259 premiers elements de base */
void init();

/* fonction de destruction d'un dictionnaire */
void destruction();

/* fonction de recherche d'un element grace a un mot dans le dictionnaire */
dict_elem* search (char* string);

/* fonction de recherche d'un mot grace a un code dans le dictionnaire */
char* search_by_code(int code);

/* fonction de recherche d'un code grace a un mot dans le dictionnaire */
unsigned int find_code (char* string);

/* fonction d'existence d'un mot dans le dictionnaire */
int exist (char* prefixe, char suffixe);

/* fonction d'insertion d'un mot dans le dictionnaire */
void insert (char* prefixe, char suffixe);

/* fonction de concatenation d'une chaine et d'un caractere */
char* cat_str_char (char* str, char car);

/* fonction affichant le dictionnaire */
void toStr();