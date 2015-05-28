typedef struct chaine_octet chaine_octet;
struct chaine_octet{
	char octet;
	chaine_octet* next;
};

void envoyer_bits(FILE* fichier, int donnee);

void envoyer_reste(FILE* fichier);

void effacer_octet(chaine_octet* tete);

char* creer_chaine(chaine_octet* tete, int taille);

char compression_fichier(FILE* fichier);