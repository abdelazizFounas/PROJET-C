#include "structuren.h"
#include "string.h"

int nb_elem = 0;

arbre_n* ptr_arbre_n = NULL;

arbre_n* tab_arbre [2048]; // declaration tableau de pointeur sur mot, indexe par le code du mot


/* fonction de creation d'un element du dictionnaire */
arbre_n* create_elem(unsigned int code, char* mot){

	/* reserve de place en memoire */
	arbre_n* newElement = (arbre_n*) malloc(sizeof(arbre_n));
	
	/* assigner les valeurs passer en parametre */
	newElement->code = code;
	newElement->mot = mot;
	newElement->freres = NULL;
	newElement->enfant = NULL;
	
	return newElement;
	
}

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base
 * et declaration du tableau de pointeur sur un sous arbre-naire du dictionnaire
 * indexe par les codes */
void init(){
	
	arbre_n* ptr_tmp;
	
	/* initialisation du premier element */
	ptr_arbre_n = create_elem(0,cat_str_char ("", 0));
	tab_arbre[0] = ptr_arbre_n;
	
	ptr_tmp = ptr_arbre_n;
	for (int i=1;i<256;i++){
		ptr_tmp->freres = create_elem(i,cat_str_char ("", i)); // dernier element de la chaine pointe sur Nil
		ptr_tmp = ptr_tmp->freres;
		tab_arbre[i] = ptr_tmp; // on met le pointeur sur l'element i dans tab[i]
	}
	
	ptr_tmp->freres = create_elem(256,NULL); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->freres;
	tab_arbre[256] = ptr_tmp;
	ptr_tmp->freres = create_elem(257,NULL); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->freres;
	tab_arbre[257] = ptr_tmp;
	ptr_tmp->freres = create_elem(258,NULL); // dernier element de la chaine pointe sur Nil
	tab_arbre[258] = ptr_tmp->freres;

	nb_elem = 259;
}

/* fonction de destruction d'un dictionnaire */
void destruction(arbre_n* ptr_a){

	/* destruction recursive debutant par le bas */
	for(int i =0; i<nb_elem;i++){
		free(tab_arbre[i]);
	}
	
	arbre_n* ptr_arbre_n = NULL;
	nb_elem = 0;
}

/* fonction de recherche d'un code dans l'arbre 
 * la fonction compare caractere par caractere
 * pour chercher la bonne branche de l'arbre
 * ou bien recherche directement dans le tableau --> a confirmer */
arbre_n* search (char* string){
	int size = strlen(string);
	arbre_n* ptr_tmp = ptr_arbre_n;
	int i=1;
	
	/* tant qu'on ne trouve pas le mot et que la taille ne soit pas depassee
	 * et que nous sommes sur une piste (caractere commun trouves)
	 * on continue de chercher en descandant dans l'arbre en conservant l'ordre lexical */
	while ((ptr_tmp!=NULL) && (i<=size) && (strcmp(string,ptr_tmp->mot)!=0)){
		ptr_tmp = arbre_mot_comp(string, ptr_tmp, i);
		//printf("AZERTYUII-%d-\n", size);
		i++;
	}
	
	return ptr_tmp;	
}

/* fonction auxiliaire de recherche d'un mot dans les freres d'un noeud en gardant l'ordre alphabetique */
arbre_n* arbre_mot_comp (char* string, arbre_n* ptr_tmp, unsigned int i){
	arbre_n* ptr_res = ptr_tmp;
	//printf("OPQSDFGkkkkk\n");

	/* on cherche le bon noeud en comparant les i premiers caracteres de la chaine */
	while (ptr_res != NULL && (strncmp(string, ptr_res->mot,i) > 0)){
		//printf("OPQSDFG\n");
		ptr_res = ptr_res->freres;
		
	}
	
	if ((ptr_res == NULL) || (strncmp(string, ptr_res->mot,i)<0)){
		return NULL; // le mot n'est pas dans le dico
	}
	else if((strcmp(string, ptr_res->mot)==0)){
		//printf("CEST BON1\n");
		return ptr_res;
	}
	else {
		//printf("CEST BON2\n");
		return ptr_res->enfant; // on va chercher dans les noeuds enfants
	}
}

/* fonction renvoyant un booleen pour dire si un mot existe dans le dico */
int exist (char* prefixe, char suffixe){
	char* string = cat_str_char(prefixe,suffixe);
	arbre_n* res = search(string);
	free(string);
	return (res!= NULL);
}

/*fonction renvoyant le code a partir d'une chaine de caractere */
unsigned int find_code (char* string){
	return (search(string))->code;
}

/* fonction d'insertion d'un code dans la liste
 * insere de facon a conserver l'ordre alphabeteique*/
void insert (char* prefixe, char suffixe){
	arbre_n *ptr_parent, *ptr_new, *ptr_cour, *ptr_suiv;
	int i=0;
	
	ptr_new = create_elem(nb_elem, cat_str_char(prefixe,suffixe));
	ptr_parent = search(prefixe);
	
	/* on cherche l'emplacement exact du mot parmis les enfants du noeud qu'on vient de trouve */
	if (ptr_parent->enfant == NULL){
		ptr_parent->enfant = ptr_new;
	}
	else{ // on cherche la place dans l'ordre alphabetique
		ptr_cour = ptr_parent->enfant;
		ptr_suiv = ptr_cour->freres;
		
		/* les mots utilises pour chercher sont de longueur minimale 3 : ascii + car + '\0' */
		while ((ptr_suiv != NULL) && (strcmp(ptr_new->mot, ptr_cour->mot)<0)){
			ptr_cour = ptr_suiv;
			ptr_suiv = ptr_suiv->freres;
		}
		
		if (ptr_cour == ptr_parent->enfant && (strcmp(ptr_new->mot, ptr_cour->mot)<0)){ // insertion en tete
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
char* cat_str_char (char* str, char car){ 
	int size = strlen(str);
	char* newStr = (char*) malloc(((size+2)*sizeof(char)));
	
	if(size>0){
		strcpy(newStr,str);
	}
	
	*(newStr+size) = car;
	*(newStr+size+1) = '\0';
	
	return newStr;
}

/* fonction affichant le dico */
void toStr(){
	for(int i =0; i<nb_elem;i++){
		if(tab_arbre[i]->code != 256 && tab_arbre[i]->code != 257 && tab_arbre[i]->code != 258){
			printf("(-%d-%lu-%s-)\n", tab_arbre[i]->code, strlen(tab_arbre[i]->mot), tab_arbre[i]->mot); // si le code n'est pas un code special on affiche le contenu de l'element
		}
	}
}

char* search_by_code(int code){
	return (tab_arbre[code])->mot;
}
