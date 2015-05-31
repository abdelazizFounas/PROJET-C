#include "structure.h"
#include "string.h"

int nb_elem = 0; // nombre d'éléments courant dans le dictionnaire

dict_elem* ptr_chaine_d = NULL; // pointeur de début du dictionnaire
dict_elem* ptr_chaine_f = NULL; // pointeur de fin du dictionnaire

/* fonction de creation d'un element du dictionnaire */
dict_elem* create_elem(unsigned int code, char* mot){
	dict_elem* newElement = (dict_elem*) malloc(sizeof(dict_elem)); // allocation memoire d'un nouvel élément vide
	
	newElement->code = code; // on place le code
	newElement->mot = mot; // on place le mot
	newElement->next = NULL; // suivant à NULL
	
	return newElement; // on renvoie le pointeur sur ce nouvel élément
}

/* fonction d'initialisation de notre dictionnaire avec les 259 premiers elements de base */
void init(){
	ptr_chaine_d = create_elem(0,cat_str_char("", 0)); // initialisation du premier element de la chaine
	ptr_chaine_f = ptr_chaine_d; // initialisation du pointeur de fin

	for (int i=1; i<256; i++){
		ptr_chaine_f->next = create_elem(i,cat_str_char("", i)); // ajout du code i
		ptr_chaine_f = ptr_chaine_f->next; // on avance
	}

	ptr_chaine_f->next = create_elem(256,NULL); // ajout du code EOF
	ptr_chaine_f = ptr_chaine_f->next;
	ptr_chaine_f->next = create_elem(257,NULL); // ajout du code de vidage du dictionnaire
	ptr_chaine_f = ptr_chaine_f->next;
	ptr_chaine_f->next = create_elem(258,NULL); // ajout du code d'incrémentation du nombre de bits
	ptr_chaine_f = ptr_chaine_f->next;
	ptr_chaine_f->next = NULL; // on ajoute NULL au next du dernier élément

	nb_elem = 259; // on a 259 éléments dans le dictionnaire
}

/* fonction de destruction d'un dictionnaire */
void destruction(){
	dict_elem* ptr_tmp; // pointeur sur élément courant
	dict_elem* ptr_next; // pointeur sur élément suivant
	
	ptr_tmp = ptr_chaine_d; // initialisation du pointeur courant
	ptr_next = ptr_chaine_d; // initialisation du pointeur suivant
	
	// tant que le pointeur suivant n'est pas NULL
	while(ptr_next != NULL){
		ptr_next = ptr_next->next; // on avance le pointeur suivant
		free(ptr_tmp); // on libere le poiteur courant
		ptr_tmp = ptr_next; // on avance le pointeur courant
	}

	nb_elem = 0; // on réinitialise le nombre d'éléments à 0
}

/* fonction de recherche d'un mot dans le dictionnaire */
dict_elem* search(char* string){
	dict_elem* ptr_tmp = ptr_chaine_d; // initialisation d'un pointeur d'élément courant
	
	int res; // variable contenant le résultat de strcmp entre le mot passé en paramètre et les mots du dictionnaire
	do{
		if(ptr_tmp->code != 256 && ptr_tmp->code != 257 && ptr_tmp->code != 258){
			res = strcmp(string,ptr_tmp->mot); // si le code n'est pas un code spécial alors on compare les chaines
		}
		
		if(res != 0){
			ptr_tmp = ptr_tmp->next; // si les chaines ne correspondent pas on avance
		}
	}while((ptr_tmp != NULL) && (res !=0)); // et ce tant que l'on est pas arrive a la fin ou que l'on ai trouve le mot

	return ptr_tmp;	// on renvoie NULL si non trouve le pointeur sur l'élément dans le cas contraire
}

/* fonction d'existence d'un mot dans le dictionnaire */
int exist (char* prefixe, char suffixe){
	char* string = cat_str_char(prefixe,suffixe); // on cree le mot a chercher par concatenation du prefixe et du suffixe
	dict_elem* res = search(string); // on recherche le mt ainsi cree
	free(string); // on libere la chaine du mot a chercher
	return (res!= NULL); // on renvoie un int correspondant a un booleen d'existence
}

/* fonction de recherche d'un code grace a un mot dans le dictionnaire */
unsigned int find_code (char* string){
	return (search(string))->code; // on renvoi le code de l'element trouve dans le dictionnaire
}

/* fonction d'insertion d'un mot dans le dictionnaire */
void insert(char* prefixe, char suffixe){
	ptr_chaine_f->next = create_elem(nb_elem, cat_str_char(prefixe,suffixe)); // on ajoute en fin un nouvel element
	ptr_chaine_f = ptr_chaine_f->next; // on met a jour le pointeur de fin
	nb_elem++; // on incremente le nombre d'elements courant
}

/* fonction de concatenation d'une chaine et d'un caractere */
char* cat_str_char (char* str, char car){ 
	int size = strlen(str); // on prend la taille du prefixe
	char* newStr = (char*) malloc(((size+2)*sizeof(char))); // on alloue taille+2 caractere pour le car et le '\0'
	
	strcpy(newStr,str); // on copie le prefixe dans cette nouvelle chaine
	
	*(newStr+size) = car; // on ajoute le suffixe
	*(newStr+size+1) = '\0'; // on ajoute le caractère de fin de chaine
	
	return newStr; // on retourne le pointeur sur chaine ainsi concatene
}

/* fonction affichant le dictionnaire */
void toStr(){
	dict_elem* ptr_tmp = ptr_chaine_d; // initialisation du poiteur de parcours
	
	// tant que l'on est pas a la fin
	while(ptr_tmp != NULL){
		if(ptr_tmp->code != 256 && ptr_tmp->code != 257 && ptr_tmp->code != 258){
			printf("(-%d-%lu-%s-)\n", ptr_tmp->code, strlen(ptr_tmp->mot), ptr_tmp->mot); // si le code n'est pas un code special on affiche le contenu de l'element
		}
		ptr_tmp = ptr_tmp->next; // on avance
	}
}

/* fonction de recherche d'un mot grace a un code dans le dictionnaire */
char* search_by_code(int code){
	dict_elem* ptr_tmp = ptr_chaine_d; // initialisation du poiteur de parcours
	
	int res; // variable contenant le résultat de la difference entre deux codes
	do{
		res = code-(ptr_tmp->code); // on compare les codes
		
		if(res != 0){
			ptr_tmp = ptr_tmp->next; // si les codes ne correspondent pas on avance
		}
	}while((ptr_tmp != NULL) && (res !=0)); // et ce tant que l'on est pas arrive a la fin ou que l'on ai trouve le code

	if(ptr_tmp != NULL){
		return ptr_tmp->mot; // si le pointeur n'est pas NULL on renvoi le mot
	}

	return NULL; // sinon on renvoi NULL
}