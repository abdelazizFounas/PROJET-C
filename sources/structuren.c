#include "structuren.h"
#include "string.h"

int nb_elem = 0;

arbre_n* ptr_arbre_n = NULL;

arbre_n* tab_arbre [4000]; // declaration tableau de pointeur sur mot, indexe par le code du mot


/* fonction de creation d'un element du dictionnaire */
arbre_n* create_elem(unsigned int code, chaine_octet* chaine, unsigned int code_pref){

	/* reserve de place en memoire */
	arbre_n* newElement = (arbre_n*) malloc(sizeof(arbre_n));
	
	/* assigner les valeurs passer en parametre */
	newElement->code = code;
	newElement->code_prefixe = code_pref;
	newElement->mot = chaine;
	newElement->freres = NULL;
	newElement->enfant = NULL;
	
	return newElement;
}

void supp_dern_elem(chaine_octet* chaine){
	chaine_octet* chaine_tmp = chaine;
	chaine_octet* chaine_prec = chaine;

	while(chaine_tmp->next != NULL){
		chaine_prec = chaine_tmp;
		chaine_tmp = chaine_tmp->next;
	}

	//free(chaine_tmp);
	chaine_prec->next = NULL;
}

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base
 * et declaration du tableau de pointeur sur un sous arbre-naire du dictionnaire
 * indexe par les codes */
arbre_n* init(){
	arbre_n* ptr_tmp;
	chaine_octet* chaine_tmp = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine_tmp->next = NULL;
	chaine_tmp->octet = 0;
	/* initialisation du premier element */
	ptr_arbre_n = create_elem(0, chaine_tmp, 0);
	tab_arbre[0] = ptr_arbre_n;
	
	ptr_tmp = ptr_arbre_n;
	for (int i=1;i<256;i++){
		chaine_tmp = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine_tmp->next = NULL;
		chaine_tmp->octet = i;
		ptr_tmp->freres = create_elem(i, chaine_tmp, 0); // dernier element de la chaine pointe sur Nil
		ptr_tmp = ptr_tmp->freres;
		tab_arbre[i] = ptr_tmp; // on met le pointeur sur l'element i dans tab[i]
	}
	
	ptr_tmp->freres = create_elem(256, NULL, 0); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->freres;
	tab_arbre[256] = ptr_tmp;
	ptr_tmp->freres = create_elem(257, NULL, 0); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->freres;
	tab_arbre[257] = ptr_tmp;
	ptr_tmp->freres = create_elem(258, NULL, 0); // dernier element de la chaine pointe sur Nil
	tab_arbre[258] = ptr_tmp->freres;

	nb_elem = 259;

	return ptr_arbre_n;
}

void effacer_octet(chaine_octet* tete){
	chaine_octet* courant = tete;
	chaine_octet* suivant = tete;
	while(suivant != NULL){
		suivant = courant->next;
		free(courant);
		courant = suivant;	
	}
}

/* fonction de destruction d'un dictionnaire */
void destruction(arbre_n* ptr_a){

	/* destruction recursive debutant par le bas */
	for(int i =0; i<nb_elem;i++){
		free(tab_arbre[i]->mot);
		free(tab_arbre[i]);
	}
	
	arbre_n* ptr_arbre_n = NULL;
	nb_elem = 0;
}

/* fonction de recherche d'un code dans l'arbre 
 * la fonction compare caractere par caractere
 * pour chercher la bonne branche de l'arbre
 * ou bien recherche directement dans le tableau --> a confirmer */
arbre_n* search (chaine_octet* string){
	int size = nb_octets(string);
	arbre_n* ptr_tmp = ptr_arbre_n;
	int i=1;
	
	/* tant qu'on ne trouve pas le mot et que la taille ne soit pas depassee
	 * et que nous sommes sur une piste (caractere commun trouves)
	 * on continue de chercher en descandant dans l'arbre en conservant l'ordre lexical */
	while ((ptr_tmp!=NULL) && i<=size && (cmp_chaine_octet(string,ptr_tmp->mot)!=0)){
		//printf("AZERTYUII-%d-\n", ptr_tmp->code);
		ptr_tmp = arbre_mot_comp(string, ptr_tmp, i);
		i++;
	}
	
	return ptr_tmp;	
}

int nb_octets(chaine_octet* chaine){
	chaine_octet* str= chaine;

	int res = 0;
	while(str != NULL){
		str = str->next;
		res++;
	}

	return res;
}

/* fonction auxiliaire de recherche d'un mot dans les freres d'un noeud en gardant l'ordre alphabetique */
arbre_n* arbre_mot_comp (chaine_octet* string, arbre_n* ptr_tmp, unsigned int i){
	arbre_n* ptr_res = ptr_tmp;
	//printf("OPQSDFGkkkkk\n");
	//printf("OPQSDFG3\n");
	/* on cherche le bon noeud en comparant les i premiers caracteres de la chaine */
	while (ptr_res != NULL && (cmp_chaine_octet_n(string, ptr_res->mot,i) > 0)){
		//printf("OPQSDFG\n");
		ptr_res = ptr_res->freres;
		
	}
	//printf("OPQSDFG4\n");
	
	if ((ptr_res == NULL) || (cmp_chaine_octet_n(string, ptr_res->mot,i)!=0)){
		//printf("CEST BON0 %d\n", string->octet);
		return NULL; // le mot n'est pas dans le dico
	}
	else if((cmp_chaine_octet(string, ptr_res->mot)==0)){
		//printf("CEST BON1\n");
		return ptr_res;
	}
	else {
		//printf("CEST BON2\n");
		return ptr_res->enfant; // on va chercher dans les noeuds enfants
	}
}

/* fonction renvoyant un booleen pour dire si un mot existe dans le dico */
int exist (chaine_octet* prefixe, unsigned int suffixe){
	//printf("15\n");
	chaine_octet* string = cat_str_char(prefixe,suffixe);
	//printf("16\n");
	arbre_n* res = search(string);
	//printf("17\n");
	effacer_octet(string);
	//printf("18\n");
	return (res!= NULL);
}

/*fonction renvoyant le code a partir d'une chaine de caractere */
unsigned int find_code (chaine_octet* string){
	return (search(string))->code;
}

/* fonction d'insertion d'un code dans la liste
 * insere de facon a conserver l'ordre alphabeteique*/
void insert (chaine_octet* prefixe, unsigned int suffixe){
	arbre_n *ptr_parent, *ptr_new, *ptr_cour, *ptr_suiv;
	int i=0;
	
	ptr_parent = search(prefixe);
	//printf("------ ");
	//afficher_octet(prefixe);
	//printf(" ------");
	chaine_octet* d = cat_str_char(prefixe, suffixe);
	//afficher_octet(d);
	ptr_new = create_elem(nb_elem, d, ptr_parent->code);
	
	/* on cherche l'emplacement exact du mot parmis les enfants du noeud qu'on vient de trouve */
	if (ptr_parent->enfant == NULL){
		ptr_parent->enfant = ptr_new;
	}
	else{ // on cherche la place dans l'ordre alphabetique
		ptr_cour = ptr_parent->enfant;
		ptr_suiv = ptr_cour->freres;
		
		/* les mots utilises pour chercher sont de longueur minimale 3 : ascii + car + '\0' */
		while ((ptr_suiv != NULL) && (cmp_chaine_octet(ptr_new->mot, ptr_cour->mot)>0)){
			ptr_cour = ptr_suiv;
			ptr_suiv = ptr_suiv->freres;
		}
		
		if (ptr_cour == ptr_parent->enfant && (cmp_chaine_octet(ptr_new->mot, ptr_cour->mot)<0)){ // insertion en tete
			ptr_parent->enfant = ptr_new;
			ptr_new->freres = ptr_cour;
		}
		else { //insertion diverse (en queue ou au milieu c'est pareil
			ptr_new->freres = ptr_suiv;
			ptr_cour->freres = ptr_new;
		}
	}
	
	tab_arbre[nb_elem] = ptr_new; //mise a jour du tableau de pointeur
	nb_elem++;
}

/* fonction concatenant une string et un caractere */
chaine_octet* cat_str_char (chaine_octet* str, unsigned int car){ 
	chaine_octet* chaine_tete = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine_tete->octet = str->octet;
	chaine_octet* chaine_tete_cour = chaine_tete;
	chaine_octet* chaine_tmp = str;
	while(chaine_tmp->next !=NULL){
		chaine_tete_cour->next = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine_tmp = chaine_tmp->next;
		chaine_tete_cour = chaine_tete_cour->next;
		chaine_tete_cour->octet = chaine_tmp->octet;
	}
	chaine_tete_cour->next = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine_tete_cour = chaine_tete_cour->next;
	chaine_tete_cour->next = NULL;
	chaine_tete_cour->octet = car;

	//afficher_octet(chaine_tete);
	
	return chaine_tete;
}

/* fonction affichant le dico */
void toStr(){
	for(int i =0; i<nb_elem;i++){
		if(tab_arbre[i]->code != 256 && tab_arbre[i]->code != 257 && tab_arbre[i]->code != 258){
			printf("(-%d-", tab_arbre[i]->code); // si le code n'est pas un code special on affiche le contenu de l'element
			afficher_octet(tab_arbre[i]->mot);
			printf("-)\n");
		}
	}
}

void afficher_octet(chaine_octet* chaine){
	chaine_octet* chaine_tmp = chaine;

	while(chaine_tmp != NULL){
		printf("%u,", chaine_tmp->octet);
		chaine_tmp = chaine_tmp->next;
	}
}

void toStrP(arbre_n* ptr, int i){
	arbre_n* ptr_tmp = ptr;

	while(ptr_tmp != NULL){
		if(ptr_tmp->code != 256 && ptr_tmp->code != 257 && ptr_tmp->code != 258){
			printf("(-%d-%d-", ptr_tmp->code, i); // si le code n'est pas un code special on affiche le contenu de l'element
			afficher_octet(ptr_tmp->mot);
			printf("-)\n");
		}
		toStrP(ptr_tmp->enfant, i+1);
		ptr_tmp = ptr_tmp->freres;
	}
}

chaine_octet* search_by_code(int code){
	if(code<nb_elem){
		return (tab_arbre[code])->mot;
	}
	return NULL;
}

int cmp_chaine_octet(chaine_octet* chaine1, chaine_octet* chaine2){
	chaine_octet* chaine1cour = chaine1;
	chaine_octet* chaine2cour = chaine2;

	while(chaine1cour != NULL && chaine2cour != NULL && (chaine1cour->octet-chaine2cour->octet) == 0){
		chaine1cour = chaine1cour->next;
		chaine2cour = chaine2cour->next;
	}

	if(chaine1cour==NULL && chaine2cour==NULL){
		return 0;
	}
	else if(chaine1cour==NULL){
		return -1;
	}
	else if(chaine2cour==NULL){
		return 1;
	}
	else{
		return chaine1cour->octet - chaine2cour->octet;
	}
}

int cmp_chaine_octet_n(chaine_octet* chaine1, chaine_octet* chaine2, int n){
	chaine_octet *chaine1n, *chaine2n, *chaine1ncour, *chaine2ncour, *chaine1cour, *chaine2cour;
	int res;

	chaine1cour = chaine1;
	chaine2cour = chaine2;

	chaine1n = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine1ncour = chaine1n;
	chaine1n->next = NULL;
	chaine1n->octet = chaine1cour->octet;
	chaine2n = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine2ncour = chaine2n;
	chaine2n->next = NULL;
	chaine2n->octet = chaine2cour->octet;

	chaine1cour = chaine1cour->next;
	chaine2cour = chaine2cour->next;

	for(int i=1; i <n ; i++){
		chaine1n->next = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine1n->next = NULL;
		chaine1n->octet = chaine1cour->octet;
		chaine2n->next = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine2n->next = NULL;
		chaine2n->octet = chaine2cour->octet;

		chaine1cour = chaine1cour->next;
		chaine2cour = chaine2cour->next;
	}

	res = cmp_chaine_octet(chaine1ncour, chaine2ncour);

	//printf("%d------\n", res);

	//afficher_octet(chaine1);
	//printf("-chaine 1\n");
	//afficher_octet(chaine2);
	//printf("-chaine 2\n");
	//afficher_octet(chaine1ncour);
	//printf("-chaine 1n\n");
	//afficher_octet(chaine2ncour);
	//printf("-chaine 1n\n");
	//printf("%d-----\n", n);

	effacer_octet(chaine1ncour);
	effacer_octet(chaine2ncour);

	return res;
}

void chaine_cat(chaine_octet* chaine1, chaine_octet* chaine2){
	chaine_octet* chaine_tmp1 = chaine1;
	chaine_octet* chaine_tmp2 = chaine2;

	while(chaine_tmp1->next != NULL){
		chaine_tmp1 = chaine_tmp1->next;
	}

	while(chaine_tmp2 != NULL){
		chaine_tmp1->next = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine_tmp1 = chaine_tmp1->next;
		chaine_tmp1->octet = chaine_tmp2->octet;
		chaine_tmp2 = chaine_tmp2->next;
	}
	chaine_tmp1->next = NULL;
}

chaine_octet* chaine_cpy(chaine_octet* str){ 
	chaine_octet* chaine_tete = (chaine_octet*) malloc(sizeof(chaine_octet));
	chaine_tete->octet = str->octet;
	chaine_octet* chaine_tete_cour = chaine_tete;
	chaine_octet* chaine_tmp = str;
	while(chaine_tmp->next !=NULL){
		chaine_tete_cour->next = (chaine_octet*) malloc(sizeof(chaine_octet));
		chaine_tmp = chaine_tmp->next;
		chaine_tete_cour = chaine_tete_cour->next;
		chaine_tete_cour->octet = chaine_tmp->octet;
	}
	chaine_tete_cour->next = NULL;

	//afficher_octet(chaine_tete);
	
	return chaine_tete;
}