/* AUTHOR : Abdelaziz FOUNAS */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mot.h"
#include "emplacement.h"
#include "maillon.h"

/* prototype de la fonction compare_mots
 * ENTREES : mot_t* pointeur sur mot représentant le premier mot a compare
 * mot_t* pointeur sur mot représentant le second mot a compare
 * SORTIE : int 0 si égalité, <0 si mot 1 plus petit, >0 si mot 1 plus grand */
int compare_mots(mot_t* mot1, mot_t* mot2){
	// on renvoie la comparaison avec les deux maillons de tete du mot
	return compare_maillons(mot1->tete_mot, mot2->tete_mot);
}

/* prototype de la fonction creation_mot
 * ENTREES : maillont_t* pointeur sur le premier maillon de tete correspondant a un mot
 * int nblig, la ligne ou le mot a été rencontré
 * int nbcol, la colonne ou le mot a été rencontré
 * SORTIE : mot_t* pointeur sur le mot ainsi cree */
mot_t* creation_mot(maillon_t* maillon, int nblig, int nbcol){
	// on alloue de l espace memoire pour le nouveau mot
	mot_t* mot_res = (mot_t *)malloc(sizeof(mot_t));
	
	// maillon precedant
	maillon_t* maillon_last;

	// on alloue de l espace memoire pour le nouvel emplacement
	emplacement_t* emplacement = (emplacement_t *)malloc(sizeof(emplacement_t));

	// tete_mot du mot resultat devient le maillon de tete recu en parametre
	mot_res->tete_mot = maillon;

	// on trouve le dernier maillon qui sera dans maillon_last
	while(maillon != NULL){
		maillon_last = maillon;
		maillon = maillon->next;
	}

	// queue_mot du mot resultat devien maillon_last que l on a trouve precedemment
	mot_res->queue_mot = maillon_last;

	// on rempli les champs de l'emplacement
	emplacement->ligne = nblig;
	emplacement->colonne = nbcol;
	emplacement->next = NULL;

	// on raccroche cet emplcement rempli au mot res
	mot_res->tete_liste = emplacement;
	mot_res->queue_liste = emplacement;

	// l attribut next du mot resultat est mis a NULL
	mot_res->next = NULL;

	// on renvoie le pointeur sur le mot resultat
	return mot_res;
}

/* prototype de la fonction affichage_mot
 * ENTREES : mot_t* pointeur sur le mot a afficher
 * SORTIE : void */
void affichage_mot(mot_t* mot){
	// on commence avec une taille de 50
	unsigned int taille = 50;

	// variable emplcement initialise a la tete de la liste des emplacements du mot
	emplacement_t* emplacement = mot->tete_liste;

	// on alloue une chaine de taille taille
	char* chaine = (char*)malloc(taille*sizeof(char));
	// chaine intermediare contenant les informations de ligne et de colonne
	char ligcol[20];

	// on convertit le mot en chaine
	char *string = convertir_en_string(mot->tete_mot);

	// on copie le mot dans la chaine
	strcpy(chaine, string);

	// on libere la chaine 
	free(string);

	// tant que l on a pas traite tous les emplacements
	while(emplacement != NULL){
		// si la taille ne suffit pas
		if(strlen(chaine) >= taille - 20){
			// ajoute 50 a la taille
			taille += 50;
			// chaine de sauvegarde
			char chaine_sauv[taille];
			// on copie la chaine a afficher dans la sauvegarde
			strcpy(chaine_sauv, chaine);
			// on libere la chaine
			free(chaine);
			// on alloue une nouvelle chaine avec la nouvelle taille
			chaine = (char*)malloc(taille*sizeof(char));
			// on restaure la copie
			strcpy(chaine, chaine_sauv);
		}

		// on met dans ligcol un format " (%d,%d)" avec la ligne et la colonne
		sprintf(ligcol, " (%d,%d)", emplacement->ligne, emplacement->colonne);
		// on concatene ligcol a la chaine a afficher
		strcat(chaine, ligcol);
		// on saute au prochain emplacement
		emplacement = emplacement->next;
	}

	// on finit par afficher la chaine
	puts(chaine);
}

/* prototype de la fonction mettre_mot
 * ENTREES : mot_t** pointeur sur le pointeur du mot correspondant au pointeur sur le debut du dictionnaire
 * mot_t* pointeur sur le mot a ajouter au dictionnaire
 * SORTIE : void */
void mettre_mot(mot_t** tete, mot_t* mot){
	// si tete pointe vers NULL
	if(*tete == NULL){
		// on insere le premier mot en tete
		*tete = mot;
	}
	else{
		// variable permettant de parcourir des mots
		mot_t* mot_courant = *tete;
		// varibles pointant vers le mot precedant
		mot_t* mot_prec;

		// tant que l on est pas a la fin et que le mot a mettre est plus grand que le mot courant 
		while(mot_courant != NULL && (compare_mots(mot, mot_courant) > 0)){
			// le mot precedant devient le mot courant
			mot_prec = mot_courant;
			// le mot courant devient le mot prochain
			mot_courant = mot_courant->next;
		}

		// si on est arrive a la fin c est que le mot a mettre est plus grand que tous les autres
		if(mot_courant == NULL){
			// on l insere alors en queue
			mot_prec->next = mot;
		}
		// sinon si on tombe sur le meme mot
		else if(compare_mots(mot, mot_courant) == 0){
			// on ajoute un emplacement au mot courant avec la ligne et la colonne du mot a mettre
			ajouter_emplacement(mot_courant, (mot->tete_liste)->ligne, (mot->tete_liste)->colonne);
			// on libere le mot
			free(mot);
		}
		// sinon
		else{
			// on fait pointe next du mot a mettre vers le mot courant
			mot->next = mot_courant;
			// si on doit inserer en tete
			if(mot_courant == *tete){
				// on change *tete avec mot
				*tete = mot;
			}
			else{
				// sinon next du mot precedant point vers le mot a mettre
				mot_prec->next = mot;
			}
		}
	}
	
}

/* prototype de la fonction affichage_dict
 * ENTREES : mot_t* pointeur sur mot correspondant au pointeur de tete du dictionnaire a afficher
 * SORTIE : void */
void affichage_dict(mot_t* tete){
	// variable permettant de parcourir la liste de mots
	mot_t* mot_courant = tete;

	// tant que l on a pas traite tous les mots
	while(mot_courant != NULL){	
		// on affiche le mot courant
		affichage_mot(mot_courant);
		// on saute au prochain mot
		mot_courant = mot_courant->next;
	}
}