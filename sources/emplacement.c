/* AUTHOR : Abdelaziz FOUNAS */

#include <stdlib.h>
#include "emplacement.h"
#include "mot.h"

/* declaration de la fonction ajouter_emplacement
 * ENTREES : mot_t* pointeur sur le mot pour lequel ajouter un emplacement
 * int nblig, la ligne ou le mot a été rencontré
 * int nbcol, la colonne ou le mot a été rencontré
 * SORTIE : void */
void ajouter_emplacement(mot_t* mot, int nblig, int nbcol){
	// allocation memoire pour un nouvel emplacement
	emplacement_t* emplacement = (emplacement_t *)malloc(sizeof(emplacement_t));

	// on rattache a la queue de la liste des emplacements le nouveau
	(mot->queue_liste)->next = emplacement;
	// la queue de la liste devient le nouveau
	mot->queue_liste = emplacement;

	// on insere dans le nouvel emplacement la ligne
	emplacement->ligne = nblig;
	// on insere dans le nouvel emplacement la colonne
	emplacement->colonne = nbcol;
	// le nouvel emplacement etant la nouvel queue, on met son attribut next a NULL
	emplacement->next = NULL;
}