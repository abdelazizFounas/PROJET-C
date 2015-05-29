
/* fonction de lecture d'un code */
int lire_code(FILE* fichier);

/* fonction d'envoi du mot dans le fichier resultat */
void envoyer_chaine (FILE* fichier, char* chaine);

/* fonction appliquant l'algorithme de decompression */
char decompression_fichier (FILE* fichier);