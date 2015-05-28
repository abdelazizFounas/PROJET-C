#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structure.h"

int nb_bits_code = 9;

unsigned int octets_a_ecrire = 0;
char nb_bits = 0;

void envoyer_bits(FILE* fichier, int donnee, char taille){
	octets_a_ecrire <<= taille;
	octets_a_ecrire += donnee;
	nb_bits += taille;

	while(nb_bits>8){
		fprintf(fichier, "%c", (octets_a_ecrire&(255<<nb_bits-8))>>(nb_bits-8));
		nb_bits -= 8;
	}
}

typedef chaine_octet struct chaine_octet;
typedef struct chaine_octet{
	char octet;
	chaine_octet* next;
}

void effacer_octet(chaine_octet* tete){
	chaine_octet* courant = tete;
	chaine_octet* suivant = tete->next;
	while(suivant != NULL){
		free(courant);
		courant = suivant;
		suivant = courant->next;
	}
	free(courant);
}

char* creer_chaine(chaine_octet* tete, int taille){
	chaine_octet* octet_courant = tete;
	chaine_octet* octet_suivant = tete->next;
	
	char* string = (char*) malloc((taille+1)sizeof(char));
	char* car_courant = string;
	
	for(int i = 0; i<taille; i++){
		(car_courant+i) = octet_courant->octet;
		courant = suivant;
		suivant = courant->next;
	}

	(car_courant+taille) = '\0';

	return string;
}

// declaration de la fonction traiter_fichier qui permettra de traiter chaque fichier
char compression_fichier(FILE* fichier){
	// si le pointeur sur le fichier n'est pas NULL
	if (fichier != NULL){
		FILE* fichier_dest = fopen("compresse.lzw", "w+");

		char* string;
		chaine_octet* tete_liste;
		chaine_octet* queue_liste;
		chaine_octet* nouveau;
		
		init();

		// on affiche le succes de l'ouverture du fichier
		//printf("Ouverture du fichier réussie : \n\n");

    	char a;
    	int taille_chaine = 1;

    	tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
    	tete_liste->next = NULL;
    	queue_liste = tete_liste;
    	fscanf(fichier, "%c", &(queue_liste->octet));

        while (!feof(fichier)) {
        	fscanf(fichier, "%c", &a);
        	string = creer_chaine(tete_liste, taille_chaine);
        	if(exist(string, a) == 1){
        		nouveau = (chaine_octet*) malloc(sizeof(chaine_octet));
        		nouveau->next = NULL;
        		nouveau->octet = a;

        		queue_liste->next = nouveau;
        		queue_liste = nouveau;
        	}
        	else{
				envoyer_bits(fichier_dest, search(string), nb_bits_code);

				insert(string, a);
				effacer_octet(tete_liste);
				tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
				tete_liste->next = NULL;
    			queue_liste = tete_liste;
    			queue_liste->octet = a;
        	}
		}

		// fermeture du fichier
		fclose(fichier);

		// on retourne 1 pour signifier la reussite de l operation
		return 1;
	}
	else{
		// sinon on affiche une erreur
		//printf("Impossible d'ouvrir le fichier : ");
		// on retourne -1 pour signifier l'erreur
		return -1;
	}
}

// declaration de la fonction main
int main(int argc, char *argv[])
{
	// declaration de la variable fichier a NULL
	FILE* fichier = NULL;

	// si l on a des arguments
	if(argc > 2){
		// initialisation de la variable i a 1, indice de l argument a traiter
		int i = 2;

		// tant que l on a encore des arguments
		while(i < argc){
			// affichage d un message indiquant le debut du traitement du fichier
			//printf("\n\nEssai ouverture du fichier : %s\n", argv[i]);

			// essai d ouverture du fichier
			fichier = fopen(argv[i], "r+");

			// si le traitement a echoue
			if(compression_fichier(fichier) == -1){
				// on affiche le nom du fichier
				//printf("%s\n", argv[i]);
			}

			// incrementation de i
			i++;
		}
	}
	else{
		// sinon on affiche que stdin est desormais la source de lecture
		//printf("\nProblème nb arg\n");
	}

	// saut de ligne
	//printf("\n");

	// on retourne le succes de la fonction main
    return EXIT_SUCCESS;
}