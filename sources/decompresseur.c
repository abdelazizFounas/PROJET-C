#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structuren.h"
#include "decompresseur.h"

int nb_bits_d_code = 9;
unsigned long int octets_lus = 0;
char nb_bits_d = 0;

int nb_elem_max_d = 512;
int nb_elem_courant_d = 259;

int taille_ficher = 17487;

int k = 0;

/* fonction de lecture d'un code */
int lire_code(FILE* fichier){
	int res;
	unsigned char lu;
	char* string;
	while(nb_bits_d < nb_bits_d_code){
		fscanf(fichier, "%c", &lu);
		octets_lus <<= 8;
		octets_lus += lu;
		nb_bits_d += 8;
		taille_ficher -= 8;
		//printf("----%lx-XXXXX\n", octets_lus);
	}
	res = (octets_lus&((nb_elem_max_d-1)<<(nb_bits_d-nb_bits_d_code)))>>(nb_bits_d-nb_bits_d_code);

	if(res != 256 && res != 257 && res != 258){
		string = search_by_code(res);
		if(string != NULL){
			printf("----%d-%d-%d-%d-%d-%s-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, string, taille_ficher);
		}
		else{
			printf("----%d-%d-%d-%d-%d-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, taille_ficher);
		}
	}
	else{
		printf("----%d-%d-%d-%d-%d-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, taille_ficher);
	}

	nb_bits_d -= nb_bits_d_code;
	return res;
}

/* fonction d'envoi du mot dans le fichier resultat */
void envoyer_chaine (FILE* fichier, char* chaine){
	fprintf(fichier,"%s", chaine);
}

/* fonction appliquant l'algorithme de decompression */
char decompression_fichier (FILE* fichier){
	unsigned int i, i_prime;
	char w[80];
	char w_prime[80];
	char a[2];
	*(a+1) = '\0';
	
	if (fichier != NULL){
		FILE* fichier_dest = fopen("decompresse.c", "w+");
	
		init();
		//toStr();
		// on affiche le succes de l'ouverture du fichier
		printf("Ouverture du fichier réussie.\n");

		k = 0;
		i = lire_code(fichier);
		*a = i;
		strcpy(w,a);
		envoyer_chaine(fichier_dest,w);
		char* string;
		while (i != 256){
			//printf("AAAAAAAAAAAA\n");
			printf("1-%d-\n", i_prime);
			k = 1;
			
			i_prime = lire_code(fichier);
			printf("14\n");
			if(i_prime==258){
				printf("2\n");
				//printf("ZZZZZZZZZZZZ\n");
				nb_elem_max_d *= 2;
				nb_bits_d_code++;
				k = 2;
				i_prime = lire_code(fichier);
			}

			if(i_prime==257){
				printf("3\n");
				//printf("EEEEEEEEEEE\n");
				*(w+strlen(w)-1) = '\0';
				envoyer_chaine(fichier_dest,w);
				toStr();
				destruction();
				init();
				printf("4\n");
				nb_elem_max_d = 512;
				nb_elem_courant_d = 259;
				nb_bits_d_code = 9;
				k = 3;
				i = lire_code(fichier);
				*a = i;
				strcpy(w,a);
				envoyer_chaine(fichier_dest,w);
				//i_prime = lire_code(fichier);
			}
			else{
				if(i_prime != 256){
					printf("5\n");
					string = search_by_code(i_prime);
				}
				else{
					string=NULL;
				}
				printf("6\n");
				if (string==NULL){
					printf("7\n");
					strcpy(w_prime,search_by_code(i));
					strcat(w_prime,a);
				}
				else {
					strcpy(w_prime,string);
				}
				printf("8\n");
				if(i_prime != 256){
					envoyer_chaine(fichier_dest,w_prime);
				}
				
				printf("9\n");
				*a = *w_prime;
				printf("ZZZZZEERRRRR\n");
				insert(w,*a);
				printf("10\n");

				i = i_prime;
				if(i!=256){
					
					if(i!=256){
						printf("11\n");
						string = search_by_code(i);
						printf("12\n");
						strcpy(w,string);
					}
				}	
			}
			nb_elem_courant_d++;
		}
		printf("13\n");
		char fin[2];
		*fin = 3;
		*(fin+1) = '\0';
		//envoyer_chaine(fichier_dest,fin);

		//printf("DDDDDDDDDD %d\n",nb_bits_d);

		fclose(fichier);
		fclose(fichier_dest);

		toStr();
		destruction();
    	nb_elem_max_d = 512;
    	nb_elem_courant_d = 259;
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