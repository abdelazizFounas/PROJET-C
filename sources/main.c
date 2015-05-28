#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structure.h"
#include "compresseur.h"


/*int nb_elem_max = 512;
int nb_elem_courant = 258;
int nb_bits_code = 9;

unsigned int octets_lus = 0;
char nb_bits = 0;

typedef struct chaine_octet chaine_octet;
typedef struct chaine_octet{
	char octet;
	chaine_octet* next;
};


int lire_code(FILE* fichier){
	char lu;
	while(nb_bits < nb_bits_code){
		fscanf(fichier, "%c", &lu);
		octets_lus <<= 8;
		octets_lus += lu;
		nb_bits += 8;
	}
	
	return (octets_lus&((nb_elem_max-1)<<(nb_bits-nb_bits_code)))>>(nb_bits-nb_bits_code);
}


// declaration de la fonction traiter_fichier qui permettra de traiter chaque fichier
char decompression_fichier(FILE* fichier){
	// si le pointeur sur le fichier n'est pas NULL
	if (fichier != NULL){
		FILE* fichier_dest = fopen("decompresse.c", "w+");

		chaine_octet* tete_liste;
		chaine_octet* queue_liste;
		chaine_octet* nouveau;
		
		int i = lire_code(fichier);

		init();

		char a = i; 

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
        		if(nb_elem_courant == nb_elem_max){
    				nb_elem_max *= 2;
    				nb_bits_code++;
    				if(nb_bits_code==15){
    					destruction();
    					init();
    					nb_elem_max = 512;
    					nb_elem_courant = 259;
    					nb_bits_code = 9; 					
    				}
    			}
    			else{

    				nb_elem_courant++;
    			}
				envoyer_bits(fichier_dest, find_code(string));

				insert(string, a);
				effacer_octet(tete_liste);
				tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
				tete_liste->next = NULL;
    			queue_liste = tete_liste;
    			queue_liste->octet = a;
    			taille_chaine = 1;
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



char* nom_fichier_lzw(char* fichier){
	int size = strlen(fichier)-2;
}*/


// declaration de la fonction main
int main(int argc, char *argv[])
{
	// declaration de la variable fichier a NULL
	FILE* fichier = NULL;
	int mode = 0;

	// si l on a des arguments
	if(argc > 2){
		// initialisation de la variable i a 1, indice de l argument a traiter
		int i = 1;

		// tant que l on a encore des arguments
		while(i < argc){
			
			if(strcmp(argv[i],"-c") == 0){
				mode = 1;
			}
			else if(strcmp(argv[i],"-d") == 0){
				mode = 2;
			}
			else{
				// affichage d un message indiquant le debut du traitement du fichier
				printf("\n\nEssai ouverture du fichier : %s.\n", argv[i]);

				// essai d ouverture du fichier
				fichier = fopen(argv[i], "r+");

				if(mode == 1){
					// si le traitement a echoue
					if(compression_fichier(fichier) == -1){
						// on affiche le nom du fichier
						printf("%s\n", argv[i]);
					}
					else{
						printf("Compression du fichier %s réussie.\n", argv[i]);
					}
				}
				/*else if(mode == 2){
					// si le traitement a echoue
					if(decompression_fichier(fichier) == -1){
						// on affiche le nom du fichier
						printf("%s\n", argv[i]);
					}
					else{
						printf("Décompression du fichier %s réussie.\n", argv[i]);
					}
				}*/
				else{
					printf("USAGE : ./main -c FICHIERS -d FICHIERS");
					break;
				}
			}
			
			// incrementation de i
			i++;
		}
	}
	else{
		// sinon on affiche que stdin est desormais la source de lecture
		printf("USAGE : ./main -c FICHIERS -d FICHIERS");
	}

	// saut de ligne
	printf("\n");

	// on retourne le succes de la fonction main
    return EXIT_SUCCESS;
}