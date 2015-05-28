/* AUTHOR : Abdelaziz FOUNAS */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "maillon.h"

// declaration d'une constante nb_bits : correspondant au nombre de bits de codage d'une lettre, 5 dans notre cas
const char nb_bits = 5;
// declaration d'une constante rang_max : rang maximum de lettre (exemple : nb_bits pour TAILLE=32)
const unsigned int rang_max = (TAILLE/nb_bits) - 1;
// declaration d'une constante mask : TYPE_T avec les nb_bits bits de poids faible a 1
const TYPE_T mask = 31;

/* prototype de la fonction char_to_num
 * ENTREES : char la lettre a transforme en TYPE_T
 * SORTIE : TYPE_T la lettre sous forme de TYPE_T */
TYPE_T char_to_num(char lettre){
	return lettre - 'a' + 1;
}

/* prototype de la fonction num_to_char
 * ENTREES : TYPE_T la lettre sous forme de TYPE_T
 * SORTIE : char la lettre sous forme de caractere */
char num_to_char(TYPE_T lettre){
	return lettre + 'a' - 1;
}

/* prototype de la fonction set_charnum
 * ENTREES : maillon_t* pointeur sur le maillon dans lequel on veut ranger une lettre
 * int le rang auquel on veut ranger la lettre
 * TYPE_T la lettre a sauvegarde
 * SORTIE : void */
void set_charnum(maillon_t *maillon, int k, TYPE_T lettre){
	// si k correspond a un rang possible
	if(k>=0 && k<=rang_max){
		// l'attribut de TYPE_T du maillon devient le meme maillon avec cinq 0 a la place de la lettre de rang k plus la lettre decaler jusque son rang
		maillon->lettres = (maillon->lettres & ~(mask << ((nb_bits*rang_max) - k*nb_bits))) + (lettre << ((nb_bits*rang_max) - k*nb_bits));
	}
}

/* prototype de la fonction get_charnum
 * ENTREES : maillon_t* pointeur sur le maillon dnas lequel on veut lire une letttre
 * int le rang de la lettre a lire
 * SORTIE : TYPE_T la lettre lu sous forme de TYPE_T */
TYPE_T get_charnum(maillon_t *maillon, int k){
	// si k correspond a un rang possible
	if(k>=0 && k<=rang_max){
		// on retourne l'attribut de TYPE_T du maillon avec toutes les lettres a 0 sauf la k eme lettre puis on decale vers la droite pour la mettre en poids faible
		return (maillon->lettres & (mask << ((nb_bits*rang_max) - k*nb_bits))) >> ((nb_bits*rang_max) - k*nb_bits);
	}
	else{
		// sinon on renvoie -1 en guise d'erreur
		return -1;
	}
}

/* prototype de la fonction compare_maillons
 * ENTREES : maillon_t* pointeur sur le premier maillon d'une liste de maillons représentant le premier mot
 * maillon_t* pointeur sur le premier maillon d'une liste de maillons représentant le second mot
 * SORTIE : int 0 si égalité, <0 si mot 1 plus petit, >0 si mot 1 plus grand */
int compare_maillons(maillon_t* maillon1, maillon_t* maillon2){
	// declaration de deux pointeurs maillon_t correspondant aux maillons courants des deux mots
	maillon_t* maillon_courant1 = maillon1;
	maillon_t* maillon_courant2 = maillon2;

	// declaration de deux pointeurs maillon_t correspondant aux maillons precedant des deux mots
	maillon_t* maillon_courant_last1;
	maillon_t* maillon_courant_last2;

	// declaration d une variable contenant le rang de la lettre courante
	int i = 0;

	// tant que l on est pas a la fin de l un des deux mots et que l on est sur deux lettres similaires
	while((maillon_courant1 != NULL) && (maillon_courant2 != NULL) && (get_charnum(maillon_courant1, i) - get_charnum(maillon_courant2, i) == 0)){
		// on commence a la lettre de rang 0
		i=0;

		// tant que l on a pas fait toutes les lettres et que l on est sur deux lettres similaires
		while((i<=rang_max) && (get_charnum(maillon_courant1, i) - get_charnum(maillon_courant2, i) == 0)){
			// on verifie les deux prochaines lettres
			i++;
		}
		// si i depasse le rang maximum
		if(i>rang_max){
			// on enregistre les maillons courants dans les maillons precedents
			maillon_courant_last1 = maillon_courant1;
			maillon_courant_last2 = maillon_courant2;

			// on passe aux maillons suivants
			maillon_courant1 = maillon_courant1->next;
			maillon_courant2 = maillon_courant2->next;
		}
	}
	
	// si i depasse le rang maximum
	if(i>rang_max){
		// si les maillons courants sont tous deux NULL
		if(maillon_courant1 == NULL && maillon_courant2 == NULL){
			// on renvoi la comparaison sur les deux dernieres lettre des maillons precedents
			return get_charnum(maillon_courant_last1, rang_max) - get_charnum(maillon_courant_last2, rang_max);
		}
		// sinon si le maillon courant 1 est NULL
		else if(maillon_courant1 == NULL){
			// alors le mot 1 est plus petit
			return -1;
		}
		else{
			// sinon il est plus grand
			return 1;
		}
		
	}
	// sinon on renvoi la comparaison sur les deux lettres courantes des maillons courants
	return get_charnum(maillon_courant1, i) - get_charnum(maillon_courant2, i);
}

/* prototype de la fonction convertir_en_maillons
 * ENTREES : char* pointeur sur chaine a convertir en maillon(s)
 * SORTIE : maillont_t* pointeur sur le maillon de tete correspondant a la chaine */
maillon_t* convertir_en_maillons(char *string){
	// variable longueur de la chainea transformee, afin de savoir le nombre de maillons necessaires
	unsigned int longueur = strlen(string);
	// variable nombre de maillons en floatant en divisant la longueur de la chaine par le nombre de lettres pouvant etre enregistre dans un maillon
	float nb = longueur/(float)(rang_max+1);
	// variable nombre de maillons en entier
	unsigned int nb_maillons = nb;

	// si le nombre de maillons en entier est inferieur au nombre de maillons en floatant
	if(nb_maillons<nb){
		// on a besoin d un maillon de plus
		nb_maillons++;
	}

	// allocation du premier maillon, le maillon de tete sera le maillon resultat a renvoye
	maillon_t* maillon_res = (maillon_t *)malloc(sizeof(maillon_t));

	// variable de maillont_t* afin creer les eventuels autres maillons
	maillon_t* maillon_courant = maillon_res;

	// variable i representant les maillons deja creer
	int i = 0;
	// variable lettre allant de 0 a la longueur de la chaine
	int lettre = 0;
	// variable nb_lettre_in allant de 0 a rang_max correspondant au nombre de lettre que l on peut mettre par maillon
	int nb_lettre_in;

	while(i < nb_maillons){
		// on initialise le nombre de lettres dans le maillons courant a 0
		nb_lettre_in = 0;

		// tant que l on est pas arrive a la fin de la chaine et a la fin du maillon
		while((lettre < longueur) && (nb_lettre_in <= rang_max)){
			// on place la lettre courante du mot dans le maillon courant
			set_charnum(maillon_courant, nb_lettre_in, char_to_num(*(string+lettre)));
			
			// phase d incrementation
			nb_lettre_in++;
			lettre++;	
		}

		// un maillon de fait, on incremente i
		i++;

		// s il nous faut encore creer un autre maillon
		if(i<nb_maillons){
			// alors on le cree a la suite du maillon courant en allouant de l espace memoire
			maillon_courant->next = (maillon_t *)malloc(sizeof(maillon_t));
			// on va traivailler desormais sur le maillon ainsi cree
			maillon_courant = maillon_courant->next;
		}
	}

	// lorsque l on a finit de convertir le mot on fait pointe l attribut next du maillon de queue a NULL
	maillon_courant->next = NULL;

	// on retourne le maillon de tete
	return maillon_res;
}

/* prototype de la fonction convertir_en_string
 * ENTREES : maillon_t* pointeur sur le maillon de tete correspondant au mot a convertir en chaine
 * SORTIE : char* pointeur sur chaine correspondant a la chaine convertit */
char* convertir_en_string(maillon_t* maillon){
	// on prend comme taille initiale 20
	int taille=20;
	// on alloue de l espace memoire a la chaine que l on convertira
	char *string_res = (char *)malloc(taille*sizeof(char));
	// on a besoin d un maillon_t* qui parcourera toute la liste de maillons
	maillon_t* maillon_courant = maillon;

	// variable de la lettre courante
	int nb = 0;
	// variable du rang courant
	int i;

	while(maillon_courant != NULL){
		// on initialise le rang courant a 0
		i=0;

		// tant que l on a pas atteint la fin du maillon ou que l on a une lettre
		while((i<=rang_max) && (get_charnum(maillon_courant, i)!=0)){
			// si la taille ne suffit pas
			if(nb==taille-1){
				// on ajoute 10 a la taille
				taille += 10;
				// varible de sauvegarde de chaine
				char chaine_sauv[taille];
				// on copie la chaine resultat dans la sauvegarde
				strcpy(chaine_sauv, string_res);
				// on libere l espace memoire de la chaine resultat
				free(string_res);
				// on alloue de nouveau de l espace memoire avec la nouvelle taille
				string_res = (char*)malloc(taille*sizeof(char));
				// on restaure la sauvegarde dans la chaine res
				strcpy(string_res, chaine_sauv);
			}

			// la lettre a l indice nb devient la lettre du rang courant dans le maillon courant
			*(string_res+nb) = num_to_char(get_charnum(maillon_courant, i));
			
			// phase d incrementation
			nb++;
			i++;
		}

		// on passe au maillon suivant
		maillon_courant = maillon_courant->next;
	}

	// on ajoute le carctere de fin a la chaine resultat
	*(string_res+nb) = '\0';

	// on renvoie le pointeur sur chaine resultat
	return string_res;
}