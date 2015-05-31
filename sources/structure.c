#include "structure.h"
#include "string.h"

int nb_elem = 0;

dict_elem* ptr_chaine_d = NULL;
dict_elem* ptr_chaine_f = NULL;

/* fonction de creation d'un element du dictionnaire */
dict_elem* create_elem(unsigned int code, char* mot){
	/* reserve de place en memoire */
	dict_elem* newElement = (dict_elem*) malloc(sizeof(dict_elem));
	
	/* assigner les valeurs passer en parametre */
	newElement->code = code;
	newElement->mot = mot;
	newElement->next = NULL;
	
	return newElement;
}

/* fonction d'initialisation de notre chaine avec les 258 premiers elements de base */
void init(){
	dict_elem* ptr_tmp;

	char* mot;
	mot = cat_str_char("", 0);
	/* initialisation du premier element de la chaine */
	ptr_chaine_d = create_elem(0,mot);
	
	/* init de tmp parcourant la chaine et parcours de la chaine en creant un nouvel element pour chaque caractere */
	ptr_tmp = ptr_chaine_d;
	for (int i=1; i<256; i++){
		/* le pointeur sur l'element suivant pointe sur le nouvel element cree grace a create_elem */
		mot = cat_str_char("", i);
		ptr_tmp->next = create_elem(i,mot); // dernier element de la chaine pointe sur Nil
		ptr_tmp = ptr_tmp->next;
	}
	ptr_tmp->next = create_elem(256,NULL); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->next;
	ptr_tmp->next = create_elem(257,NULL); // dernier element de la chaine pointe sur Nil
	ptr_tmp = ptr_tmp->next;
	ptr_tmp->next = create_elem(258,NULL); // dernier element de la chaine pointe sur Nil
	ptr_chaine_f = ptr_tmp->next;
	ptr_chaine_f->next = NULL;

	nb_elem = 259;
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
	/* initialisation du ptr de parcours */
	ptr_tmp = ptr_chaine_d;
	
	/* tant qu'on n'est pas en fin de liste et qu'on ne trouve pas le bon prefixe, on avance 
	 * note : le strcmp renvoie 0 si les chaines sont égales, nb>0 si s1 est superieur a s2 dans ordre lexicographique
	 * sinon elle renvoie un nb negatif */
	int res = strcmp(string,ptr_tmp->mot);
	while((ptr_tmp != NULL) && (res !=0)){
		if(ptr_tmp->code == 256 || ptr_tmp->code == 257 || ptr_tmp->code == 258){
			ptr_tmp = ptr_tmp->next;
		}
		else{
			res = strcmp(string,ptr_tmp->mot);
			if(res != 0){
				ptr_tmp = ptr_tmp->next;
			}
		}
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
	ptr_chaine_f->next = create_elem(nb_elem, cat_str_char(prefixe,suffixe));
	ptr_chaine_f = ptr_chaine_f->next;
	ptr_chaine_f->next = NULL;
	nb_elem++;
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
	dict_elem* ptr_tmp = ptr_chaine_d;
	
	while(ptr_tmp != NULL){
		if(ptr_tmp->code != 256 && ptr_tmp->code != 257 && ptr_tmp->code != 258){
			printf("(-%d-%lu-%s-)\n", ptr_tmp->code, strlen(ptr_tmp->mot), ptr_tmp->mot);
		}
		ptr_tmp = ptr_tmp->next;
	}
}

char* search_by_code(int code){
	dict_elem* ptr_tmp;
	/* initialisation du ptr de parcours */
	ptr_tmp = ptr_chaine_d;
	
	/* tant qu'on n'est pas en fin de liste et qu'on ne trouve pas le bon prefixe, on avance 
	 * note : le strcmp renvoie 0 si les chaines sont égales, nb>0 si s1 est superieur a s2 dans ordre lexicographique
	 * sinon elle renvoie un nb negatif */
	int res = code-(ptr_tmp->code);
	while((ptr_tmp != NULL) && (res !=0)){
		res = code-(ptr_tmp->code);
		if(res != 0){
			ptr_tmp = ptr_tmp->next;
		}
	}
	if(ptr_tmp != NULL){
		return ptr_tmp->mot;
	}
	return NULL;
}