#include "structure.h"
#include "string.h"

int nb_elem = 0;

dict_elem* ptr_chaine_d = NULL;
dict_elem* ptr_chaine_f = NULL;

/* fonction de creation d'un element du dictionnaire */
dict_elem* create_elem(unsigned int code, char* prefixe, char suffixe, unsigned int code_pref, unsigned int code_suf){
	/* reserve de place en memoire */
	dict_elem* newElement = (dict_elem*) malloc(sizeof(dict_elem));
	
	/* assigner les valeurs passer en parametre */
	newElement->code = code;
	newElement->code_pref = code_pref;
	newElement->code_suf = code_suf;
	
	/* trouver le code des prefixes et suffixes --> utiliser find */
	newElement->prefixe = prefixe;
	newElement->suffixe = suffixe;
	
	return newElement;
}

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base */
void init(){
	dict_elem* ptr_tmp;

	/* initialisation du premier element de la chaine */
	ptr_chaine_d = create_elem(0,"",0,0,0);
	ptr_chaine_f = ptr_chaine_d;
	
	/* init de tmp parcourant la chaine et parcours de la chaine en creant un nouvel element pour chaque caractere */
	ptr_tmp = ptr_chaine_d;
	for (int i=1; i<256; i++){
		/* le pointeur sur l'element suivant pointe sur le nouvel element cree grace a create_elem */
		ptr_tmp->next = create_elem(i,"",i,0,i); // dernier element de la chaine pointe sur Nil
		ptr_tmp = ptr_tmp->next;
	}

	ptr_tmp->next = create_elem(256,"",0,0,256); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->next;
	ptr_tmp->next = create_elem(257,"",0,0,257); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->next;

	ptr_chaine_f = ptr_tmp;
	ptr_chaine_f->next = NULL;

	nb_elem = 257;
}

/* fonction de destruction d'un dictionnaire */
void destruction(){
	dict_elem* ptr_tmp;
	dict_elem* ptr_next;
	
	ptr_tmp = ptr_chaine_d;
	ptr_next = ptr_chaine_d->next;
	
	while(ptr_next != NULL){
		free(ptr_tmp);
		ptr_tmp = ptr_next;
		ptr_next = ptr_next->next;
	}
	free(ptr_tmp);

	nb_elem = 0;
}

/* fonction longueur de chaine */
unsigned int length (dict_elem* ptr_dict_elem){
	return (ptr_chaine_f->code)+1;
}

/* fonction de recherche d'un code dans la liste */
dict_elem* search(char* string){
	dict_elem* ptr_tmp;
	char* chaine;
	/* initialisation du ptr de parcours */
	ptr_tmp = ptr_chaine_d;
	
	/* tant qu'on n'est pas en fin de liste et qu'on ne trouve pas le bon prefixe, on avance 
	 * note : le strcmp renvoie 0 si les chaines sont égales, nb>0 si s1 est superieur a s2 dans ordre lexicographique
	 * sinon elle renvoie un nb negatif */
	chaine = cat_str_char(ptr_tmp->prefixe,ptr_tmp->suffixe);
	while((ptr_tmp != NULL) && (strcmp(string,chaine) != 0)){
		free(chaine);
		ptr_tmp = ptr_tmp->next;
		if(ptr_tmp != NULL){
			chaine = cat_str_char(ptr_tmp->prefixe,ptr_tmp->suffixe);
		}
	}
	if(ptr_tmp != NULL){
		free(chaine);
	}

	return ptr_tmp;	
}

/* fonction exist renvoyant vrai si la chaine lu est dans le dico, sinon 0 */
int exist (char* prefixe, char suffixe){
	char* string = cat_str_char(prefixe,suffixe);
	dict_elem* res = search(string);
	free(string);
	return (res!= NULL);
}


/* fonction find_code renvoyant le code du mot lu passe par la chaine de caractere en parametre */
unsigned int find_code (char* string){
	return (search(string))->code;
}

/* fonction d'insertion en queue d'un code dans la liste*/
void insert(char* prefixe, char suffixe){
	nb_elem++;
	dict_elem* intermediaire = create_elem(nb_elem, prefixe, suffixe, find_code(prefixe), suffixe);
	ptr_chaine_f->next = intermediaire;
	ptr_chaine_f = ptr_chaine_f->next;
	intermediaire->next = NULL;
}

/* fonction de concatenation d'une chaine et d'un caractere */
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

void toStr(){
	dict_elem* ptr_tmp;
	/* initialisation du ptr de parcours */
	ptr_tmp = ptr_chaine_d;
	
	while(ptr_tmp != NULL){
		printf("(-%lu-,-%s-,-%c-,-%d-) ", strlen(ptr_tmp->prefixe), ptr_tmp->prefixe, ptr_tmp->suffixe,ptr_tmp->code);
		ptr_tmp = ptr_tmp->next;
	}
}