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

/* fonction de lecture d'un code */
unsigned int lire_code(FILE* fichier){
	int res;
	unsigned char lu;
	char* string;
	while(nb_bits_d < nb_bits_d_code){
		fscanf(fichier, "%c", &lu);
		octets_lus <<= 8;
		octets_lus += lu;
		nb_bits_d += 8;
		//printf("----%lx-XXXXX\n", octets_lus);
	}
	res = (octets_lus&((nb_elem_max_d-1)<<(nb_bits_d-nb_bits_d_code)))>>(nb_bits_d-nb_bits_d_code);

	/*if(res != 256 && res != 257 && res != 258){
		string = search_by_code(res);
		if(string != NULL){
			printf("-%s", string);
			//printf("----%d-%d-%d-%d-%d-%s-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, string, taille_ficher);
		}
		else{
			//printf("----%d-%d-%d-%d-%d-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, taille_ficher);
		}
	}
	else{
		//printf("----%d-%d-%d-%d-%d-%d-XXXXX\n", res, nb_bits_d, nb_bits_d_code, nb_elem_courant_d, k, taille_ficher);
	}*/

	//printf("-%d-%d-\n", res, nb_elem_courant_d);

	nb_bits_d -= nb_bits_d_code;
	return res;
}

/* fonction d'envoi du mot dans le fichier resultat */
void envoyer_chaine (FILE* fichier, chaine_octet* chaine){
	chaine_octet* chaine_tmp = chaine;
	while(chaine_tmp != NULL){
		fputc(chaine_tmp->octet, fichier);
		chaine_tmp = chaine_tmp->next;
	}
}



/* fonction appliquant l'algorithme de decompression */
char decompression_fichier (FILE* fichier){
	unsigned int i, i_prime;
	chaine_octet* w = NULL;
	chaine_octet* w_prime = NULL;
	chaine_octet* a = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine_octet* res;
	chaine_octet* tmp;
	a->next = NULL;
	
	if (fichier != NULL){
		FILE* fichier_dest = fopen("decompresse.c", "w+");
	
		init();
		//toStr();
		// on affiche le succes de l'ouverture du fichier
		printf("Ouverture du fichier réussie.\n");

		i = lire_code(fichier);
		a->octet = i;
		w = (chaine_octet*) malloc(sizeof(chaine_octet));
		w->next = NULL;
		w->octet = i;
		envoyer_chaine(fichier_dest,w);
		while (i != 256){
			//printf("AAAAAAAAAAAA\n");
			//printf("1-%d-\n", i_prime);

			i_prime = lire_code(fichier);
			//printf("14\n");
			if(i_prime==258){
				//printf("2\n");
				//printf("ZZZZZZZZZZZZ\n");
				nb_elem_max_d *= 2;
				nb_bits_d_code++;
				i_prime = lire_code(fichier);
			}

			if(i_prime==257){
				//printf("3\n");
				//printf("EEEEEEEEEEE\n");
				w = supp_dern_elem(w);
				envoyer_chaine(fichier_dest,w);
				//toStr();
				destruction();
				init();
				//printf("4\n");
				nb_elem_max_d = 512;
				nb_elem_courant_d = 259;
				nb_bits_d_code = 9;
				i = lire_code(fichier);
				a->octet = i;
				effacer_octet(w);
				w = (chaine_octet*) malloc(sizeof(chaine_octet));
				w->next = NULL;
				w->octet = i;
				envoyer_chaine(fichier_dest,w);
				//i_prime = lire_code(fichier);
			}
			else{
				if(i_prime != 256){
					//printf("5\n");
					res = search_by_code(i_prime);
				}
				else{
					res=NULL;
				}
				//printf("6\n");
				if (res==NULL){
					//printf("7\n");
					effacer_octet(w_prime);
					w_prime = chaine_cpy(search_by_code(i));
					chaine_cat(w_prime,a);
				}
				else {
					effacer_octet(w_prime);
					w_prime = chaine_cpy(res);
				}
				//printf("8\n");
				if(i_prime != 256){
					envoyer_chaine(fichier_dest,w_prime);
				}
				
				//printf("9\n");
				a->octet = w_prime->octet;
				//printf("ZZZZZEERRRRR\n");
				insert(w,a->octet);
				//printf("10\n");

				i = i_prime;
				if(i!=256){
					
					if(i!=256){
						//printf("12\n");
						//effacer_octet(w);
						w = search_by_code(i);
					}
				}	
			}
			nb_elem_courant_d++;
		}
		//printf("13\n");
		//envoyer_chaine(fichier_dest,fin);

		//printf("DDDDDDDDDD %d\n",nb_bits_d);

		fclose(fichier);
		fclose(fichier_dest);

		//toStr();
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