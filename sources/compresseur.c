#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structure.h"
#include "compresseur.h"

int nb_elem_max = 512;
int nb_elem_courant = 258;

int nb_bits_code = 9;

unsigned int octets_a_ecrire = 0;
char nb_bits = 0;

void envoyer_bits(FILE* fichier, int donnee){
	int res;
	octets_a_ecrire <<= nb_bits_code;
	octets_a_ecrire += donnee;
	nb_bits += nb_bits_code;

	while(nb_bits>=8){
		res = (octets_a_ecrire&(255<<(nb_bits-8)))>>(nb_bits-8);
		fprintf(fichier, "%c", res);
		nb_bits -= 8;
	}
}

void envoyer_reste(FILE* fichier){
	if(nb_bits > 0){
		octets_a_ecrire <<= (8-nb_bits);
		fprintf(fichier, "%c", (octets_a_ecrire&255));
		nb_bits = 0;
	}
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
	
	char* string = (char*) malloc((taille+1)*sizeof(char));
	char* car_courant = string;
	int i = 0;
	for(i = 0; i<taille-1; i++){
		*(car_courant+i) = octet_courant->octet;
		octet_courant = octet_suivant;
		octet_suivant = octet_suivant->next;
	}
	*(car_courant+i) = octet_courant->octet;

	*(car_courant+taille) = '\0';

	return string;
}

// declaration de la fonction traiter_fichier qui permettra de traiter chaque fichier
char compression_fichier(FILE* fichier){
	// si le pointeur sur le fichier n'est pas NULL
	if (fichier != NULL){
		FILE* fichier_dest = fopen("compresse.lzw", "w+");

		chaine_octet* tete_liste;
		chaine_octet* queue_liste;
		chaine_octet* nouveau;
		
		init();
		// on affiche le succes de l'ouverture du fichier
		printf("Ouverture du fichier réussie.\n");

    	char a;
    	int taille_chaine = 1;

    	tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
    	tete_liste->next = NULL;
    	queue_liste = tete_liste;
    	fscanf(fichier, "%c", &(queue_liste->octet));

        while (!feof(fichier)) {
        	fscanf(fichier, "%c", &a);
        	char* string = creer_chaine(tete_liste, taille_chaine);
        	if(exist(string, a) != 0){
        		taille_chaine++;
        		nouveau = (chaine_octet*) malloc(sizeof(chaine_octet));
        		nouveau->next = NULL;
        		nouveau->octet = a;

        		queue_liste->next = nouveau;
        		queue_liste = nouveau;        		
        	}
        	else{
        		nb_elem_courant++;
    			
				envoyer_bits(fichier_dest, find_code(string));

				insert(string, a);
				effacer_octet(tete_liste);
				tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
				tete_liste->next = NULL;
    			queue_liste = tete_liste;
    			queue_liste->octet = a;
    			taille_chaine = 1;

    			if(nb_elem_courant == nb_elem_max){
    				nb_elem_max *= 2;
    				nb_bits_code++;
    				if(nb_bits_code==16){

    					envoyer_bits(fichier_dest, 257);
    					printf("EEEEEEEEEEE\n");
    					destruction();
    					init();
    					nb_elem_max = 512;
    					nb_elem_courant = 258;
    					nb_bits_code = 9; 					
    				}
    			}
        	}
        	
        	free(string);
		}

		envoyer_bits(fichier_dest, 256);
		envoyer_reste(fichier_dest);

		// fermeture du fichier
		fclose(fichier);
		fclose(fichier_dest);

		//toStr();
		// on retourne 1 pour signifier la reussite de l operation
		return 1;
	}
	else{
		// sinon on affiche une erreur
		printf("Impossible d'ouvrir le fichier : ");
		// on retourne -1 pour signifier l'erreur
		return -1;
	}
}