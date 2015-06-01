#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "structuren.h"
#include "compresseur.h"

int nb_elem_max = 512;
int nb_elem_courant = 259;

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

unsigned int char_to_int(unsigned char car){
	int bit = 1;
	unsigned int res=0;
	for(int i = 0; i<8; i++){
		res+=(bit & car);
		bit <<= 1;
	}
	return res;
}

// declaration de la fonction traiter_fichier qui permettra de traiter chaque fichier
char compression_fichier(FILE* fichier){
	// si le pointeur sur le fichier n'est pas NULL
	if (fichier != NULL){
		FILE* fichier_dest = fopen("compresse.lzw", "w+");

		chaine_octet* tete_liste;
		chaine_octet* queue_liste;
		
		arbre_n* arbre = init();
		// on affiche le succes de l'ouverture du fichier
		printf("Ouverture du fichier réussie.\n");

    	unsigned char a, b;
    	int taille_chaine = 1;

    	tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
    	tete_liste->next = NULL;
    	queue_liste = tete_liste;
    	b = fgetc(fichier);
    	queue_liste->octet = b;
    	//printf("1\n");

        while (!feof(fichier)) {
        	//toStr();
        	if(nb_elem_courant == nb_elem_max){
				if(nb_bits_code==11){
					envoyer_bits(fichier_dest, a);
					//printf("-%d-%d-%c-\n", a, nb_elem_courant,a);
					//printf("--------%d-%d---\n", a, nb_elem_courant);
					envoyer_bits(fichier_dest, 257);
					//printf("-%d-%d-\n", 257, nb_elem_courant);
					//printf("--------%d-%d---\n", 257, nb_elem_courant);
					//printf("EEEEEEEEEEE\n");
					//toStr();
					destruction();
					arbre = init();
					b = fgetc(fichier);
    				queue_liste->octet = b;
					nb_elem_max = 512;
					nb_elem_courant = 259;
					nb_bits_code = 9;
					taille_chaine = 1;			
				}
				else{
					//printf("11\n");
					envoyer_bits(fichier_dest, 258);
					//printf("-%d-%d-\n", 258, nb_elem_courant);
					//printf("--------%d-%d---\n", 258, nb_elem_courant);
					nb_elem_max *= 2;
					nb_bits_code++;
				}
			}
        	//printf("2\n");
        	a = fgetc(fichier);
        	//printf("3\n");
        	//printf("4 \n");
        	if(exist(tete_liste, a) != 0){
        		//printf("5\n");
        		//printf("-%c-\n", a);
        		taille_chaine++;
        		queue_liste->next = (chaine_octet*) malloc(sizeof(chaine_octet));
        		queue_liste = queue_liste->next;
        		queue_liste->next = NULL;
        		queue_liste->octet = a;
        	}
        	else{
        		
        		//printf("6-\n");
    			//afficher_octet(tete_liste);
    			//printf("-%d-\n", a);
    			insert(tete_liste, a);
    			//toStr();
    			//toStrP(arbre, 0);
				envoyer_bits(fichier_dest, find_code(tete_liste));
				//printf("-%d-%d-\n", find_code(tete_liste), nb_elem_courant);

				//printf("7\n");

				
				nb_elem_courant++;
				//printf("8\n");
				effacer_octet(tete_liste);
				//printf("9\n");
				tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
				//printf("10\n");
				tete_liste->next = NULL;
    			queue_liste = tete_liste;
    			queue_liste->octet = a;
    			taille_chaine = 1;


        	}
        	//printf("taille_chaine : %d.\n", taille_chaine);
        	//printf("12\n");
		}
		//printf("taille_chaine : %d.\n", taille_chaine);

		//supp_dern_elem(tete_liste);

		//envoyer_bits(fichier_dest, find_code(tete_liste));
		effacer_octet(tete_liste);
		//printf("13\n");
		envoyer_bits(fichier_dest, 256);
		//printf("-%d-%d-\n", 256, nb_elem_courant);
		//printf("--------%d-%d---\n", 256, nb_elem_courant);
		//printf("14\n");
		envoyer_reste(fichier_dest);
		//printf("15\n");

		// fermeture du fichier
		fclose(fichier);
		fclose(fichier_dest);
		
		//toStr();
		destruction();
		//printf("16\n");
    	nb_elem_max = 512;
    	nb_elem_courant = 259;
    	nb_bits_code = 9; 

		
		//printf("17\n");
		// on retourne 1 pour signifier la reussite de l operation
		return 1;
	}
	else{
		// sinon on affiche une erreur
		printf("Impossible d'ouvrir le fichier : ");
		// on retourne -1 pour signifier l'erreur
		return -1;
	}

	//printf("%u\n", char_to_int('a'));
	
	/*chaine_octet* tete_liste;
	chaine_octet* queue_liste;

	tete_liste = (chaine_octet*) malloc(sizeof(chaine_octet));
    tete_liste->next = NULL;
    queue_liste = tete_liste;
    queue_liste->octet = 'a';
	
	arbre_n* arbre = init();
	insert(tete_liste, 'l');

	queue_liste->next = (chaine_octet*) malloc(sizeof(chaine_octet));
	queue_liste = queue_liste->next;
	queue_liste->next = NULL;
	queue_liste->octet = 'l';

	insert(tete_liste, 'l');

	queue_liste->next = (chaine_octet*) malloc(sizeof(chaine_octet));
	queue_liste = queue_liste->next;
	queue_liste->next = NULL;
	queue_liste->octet = 'l';

	toStrP(arbre, 0);
	if(exist (tete_liste, 'l')){
		printf("BON");
	}

	printf("%d", find_code(tete_liste));*/
	return 1;
}