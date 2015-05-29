#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structure.h"
#include "decompresseur.h"

int nb_bits_d_code = 9;
unsigned int octets_lus = 0;
char nb_bits_d = 0;

int nb_elem_max_d = 512;
int nb_elem_courant_d = 258;

/* fonction de lecture d'un code */
int lire_code(FILE* fichier){
	int res;
	unsigned char lu;
	while(nb_bits_d < nb_bits_d_code){
		fscanf(fichier, "%c", &lu);
		octets_lus <<= 8;
		octets_lus += lu;
		nb_bits_d += 8;
	}
	res = (octets_lus&((nb_elem_max_d-1)<<(nb_bits_d-nb_bits_d_code)))>>(nb_bits_d-nb_bits_d_code);
	nb_bits_d -= nb_bits_d_code;
	return res;
}

/* fonction d'envoi du mot dans le fichier resultat */
void envoyer_chaine (FILE* fichier, char* chaine){
	fprintf(fichier, "%s", chaine);
}

/* fonction appliquant l'algorithme de decompression */
char decompression_fichier (FILE* fichier){
	unsigned int i, i_prime;
	char w[20];
	char w_prime[20];
	char a[2];
	*(a+1) = '\0';
	
	if (fichier != NULL){
		printf("1\n");
		FILE* fichier_dest = fopen("decompresse.d", "w+");
	
		init();
		printf("2\n");
		// on affiche le succes de l'ouverture du fichier
		printf("Ouverture du fichier réussie.\n");

		i = lire_code(fichier);
		printf("3\n");
		*a = i;
		strcpy(w,a);
		printf("4\n");
		envoyer_chaine(fichier_dest,w);
		printf("5\n");
		char* string;
		while (i != 256){
			printf("6\n");
			if(i==257){
				printf("EEEEEEEEEEE\n");
				destruction();
				init();
				nb_elem_max_d = 512;
				nb_elem_courant_d = 258;
				nb_bits_d_code = 9;
			}
			else{
				printf("7\n");
				i_prime = lire_code(fichier);
				printf("8\n");
				string = search_by_code(i_prime);
				printf("9\n");
				if (string==NULL){
					printf("9\n");
					strcpy(w_prime,search_by_code(i));
					printf("10\n");
					strcat(w_prime,a);
				}
				else {
					printf("----  \n");

					printf("-%s-\n", search_by_code(23));

					strcpy(w_prime,string);
					printf("17\n");
				}
				printf("12\n");
				envoyer_chaine(fichier_dest,w_prime);
				printf("13\n");
				*a = *w_prime;
				insert(w,*a);
				printf("14\n");
				i = i_prime;
				strcpy(w,search_by_code(i));
				printf("15\n");
				nb_elem_courant_d++;

				if(nb_elem_courant_d == nb_elem_max_d){
					printf("16\n");
					nb_elem_max_d *= 2;
					nb_bits_d_code++;
	    		}
			}
		}

		fclose(fichier);
		fclose(fichier_dest);

		destruction();
    	nb_elem_max_d = 512;
    	nb_elem_courant_d = 258;
    	nb_bits_d_code = 9; 

		return 1;
	}
	else{
		// sinon on affiche une erreur
		printf("Impossible d'ouvrir le fichier : ");
		// on retourne -1 pour signifier l'erreur
		return -1;
	}
}




