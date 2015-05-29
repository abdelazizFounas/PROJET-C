#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structure.h"
#include "compresseur.h"
#include "decompresseur.h"

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
				else if(mode == 2){
					// si le traitement a echoue
					if(decompression_fichier(fichier) == -1){
						// on affiche le nom du fichier
						printf("%s\n", argv[i]);
					}
					else{
						printf("Décompression du fichier %s réussie.\n", argv[i]);
					}
				}
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