#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DIMENSION 12

struct Grille{
  unsigned int dimension;
  char lettres [MAX_DIMENSION][MAX_DIMENSION];
  bool choisi [MAX_DIMENSION][MAX_DIMENSION];
};

/**
 * Trouver la dimension de la grille en regardant pour le premier
 * caractère de saut de ligne ('\r\n').
 *
 * @param lettresGrille le tableau contenant tous les caractères du fichier
 * @return la dimension de la grille
 */
int trouverDimension(const char lettresGrille[]){
  bool trouver = false;
  int dimension = 0;
  int i =0;
  while(!trouver){
    if(lettresGrille[i] == '\r' || lettresGrille[i] == '\n'){
      trouver = true;
    } else {
      dimension++, i++;
    }
  }
  return dimension;
}

/**
 * Remplir les lettres constituant la grille dans la variable passé par
 * référencement nommé grille et les placer aux bonnes positions dans 
 * le tableau a deux dimension.
 *  
 * @param grille Pointeur vers la grille ou l'on doit ajouter les lettres
 * @param lettresGrille Le tableau contenant tous les caractères du fichier
 * @param lettreCourante Pointeur vers la lettre courante dans la lecture du fichier
 */
void remplirGrille(struct Grille* grille,const char lettresGrille[], int *lettreCourante){
  int y=0;
  int x=0;
  while(y < grille->dimension){
    if(x < grille->dimension){
      grille->lettres[x][y] = lettresGrille[*lettreCourante];
      x++, (*lettreCourante)++;
    } else {
      x = 0;
      y++;

      while(lettresGrille[*lettreCourante] == '\r' || lettresGrille[*lettreCourante] == '\n'){
        (*lettreCourante)++;
      }
    }
  }
}

/**
 * Stock les mots a trouver dans la grille en gardant le caractère
 * '\n' pour les séparer l'un des autres.
 *  
 * @param lettresGrille Le tableau contenant tous les caractères du fichier
 * @param lettreCourante Pointeur vers la lettre courante dans la lecture du fichier
 * @param motsATrouver Le tableau pour stocker les mots cachés
 */
void stockerMotCache(const char lettresGrille[], int *lettreCourante, char motsATrouver[]){
  int i =0;
  while(lettresGrille[*lettreCourante] != '\0'){
    if(lettresGrille[*lettreCourante] != '\r'){
      motsATrouver[i] = lettresGrille[*lettreCourante];
      i++;
    }
    (*lettreCourante)++;
  }
}

/**
 * Retire le mot a trouver du tableau selon la position actuel dans la
 * liste des mots a trouver.
 *  
 * @param motsATrouver Le tableau contenant les mots cachés
 * @param position Pointeur vers la position dans la liste des mots cachés
 * @param motCourant Le tableau pour stocker le mot courant
 */
void retirerMotListe(const char motsATrouver[], int *position, char motCourant[]) {
    int i = 0;
    while (motsATrouver[*position] != '\n' && motsATrouver[*position] != '\0') {
        motCourant[i] = motsATrouver[*position];
        i++, (*position)++;
    }

    motCourant[i] = '\0';  

    if (motsATrouver[*position] == '\n') {
      (*position)++; 
    }
}

/**
 * Permet d'inverser le mot a trouver en but de trouver si ce dernier
 * est a l'envers dans la grille.
 *  
 * @param motCourant Le mot a cherché dans la grille
 */
void inverserMot(char *motCourant) {
  int longueur = strlen(motCourant);
  int debut = 0;
  int fin = longueur - 1;

  while (debut < fin) {
    char tmp = motCourant[debut];
    motCourant[debut] = motCourant[fin];
    motCourant[fin] = tmp;
    debut++;
    fin--;
    }
}

/**
 * Permet de parcourir, de façon horizontal, dans la grille pour trouver si
 * le mot y est présent. Si c'est le cas, les lettres sont marqués comme
 * utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motCourant Le mot courant a trouver dans la grille
 * @param y Le y de la lettre du début
 * @param x Le x de la lettre du début
 */
void chercherHorizontal(struct Grille* grille, const char motCourant[], int y, int x){
  int longueurMot = strlen(motCourant);
  int debut = x;
  bool estTrouve = false;
  if(x+longueurMot <= grille->dimension){
    if(grille->lettres[x][y] == motCourant[0]){
      estTrouve = true;

      for(int i = 0; i < longueurMot && estTrouve; i++){
        if(grille->lettres[x][y] != motCourant[i]){
          estTrouve = false;
        } else {
          x++;
        }
      }
    }
  }

  if(estTrouve){
    for(int i = 0; i < longueurMot; i++){
      grille->choisi[debut][y] = true;
      debut++;
    }
  }
}

/**
 * Permet de parcourir, de façon vertical, dans la grille pour trouver si
 * le mot y est présent. Si c'est le cas, les lettres sont marqués comme
 * utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motCourant Le mot courant a trouver dans la grille
 * @param y Le y de la lettre du début
 * @param x Le x de la lettre du début
 */
void chercherVertical(struct Grille* grille, const char motCourant[], int y, int x){
  int longueurMot = strlen(motCourant);
  int debut = y;
  bool estTrouve = false;
  if(y+longueurMot <= grille->dimension){
    if(grille->lettres[x][y] == motCourant[0]){
      estTrouve = true;

      for(int i = 0; i < longueurMot && estTrouve; i++){
        if(grille->lettres[x][y] != motCourant[i]){
          estTrouve = false;
        } else {
          y++;
        }
      }
    }
  }

  if(estTrouve){
    for(int i = 0; i < longueurMot; i++){
      grille->choisi[x][debut] = true;
      debut++;
    }
  }
}

/**
 * Permet de parcourir, de façon diagonal, dans la grille pour trouver si
 * le mot y est présent. Si c'est le cas, les lettres sont marqués comme
 * utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motCourant Le mot courant a trouver dans la grille
 * @param y Le y de la lettre du début
 * @param x Le x de la lettre du début
 */
void chercherDiagonalDroite(struct Grille* grille, const char motCourant[], int y, int x){
  int longueurMot = strlen(motCourant);
  int debutY = y;
  int debutX = x;
  bool estTrouve = false;
  if(y+longueurMot <= grille->dimension && x+longueurMot <= grille->dimension){
    if(grille->lettres[x][y] == motCourant[0]){
      estTrouve = true;

      for(int i = 0; i < longueurMot && estTrouve; i++){
        if(grille->lettres[x][y] != motCourant[i]){
          estTrouve = false;
        } else {
          y++, x++;
        }
      }
    }
  }

  if(estTrouve){
    for(int i = 0; i < longueurMot; i++){
      grille->choisi[debutX][debutY] = true;
      debutX++, debutY++;
    }
  }
}

/**
 * Permet de parcourir, de façon diagonal, dans la grille pour trouver si
 * le mot y est présent. Si c'est le cas, les lettres sont marqués comme
 * utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motCourant Le mot courant a trouver dans la grille
 * @param y Le y de la lettre du début
 * @param x Le x de la lettre du début
 */
void chercherDiagonalGauche(struct Grille* grille, const char motCourant[], int y, int x){
  int longueurMot = strlen(motCourant);
  int debutY = y;
  int debutX = x;
  bool estTrouve = false;
  if(y-longueurMot+1 <= 0 && x-longueurMot+1 <= 0){
    if(grille->lettres[x][y] == motCourant[0]){
      estTrouve = true;

      for(int i = 0; i < longueurMot && estTrouve; i++){
        if(grille->lettres[x][y] != motCourant[i]){
          estTrouve = false;
        } else {
          y--, x--;
        }
      }
    }
  }

  if(estTrouve){
    for(int i = 0; i < longueurMot; i++){
      grille->choisi[debutX][debutY] = true;
      debutX--, debutY--;
    }
  }
}


/**
 * Permet de parcourir le mot courant a trouver et de chercher
 * ce derniers dans la grille pour inscrire les lettres utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motCourant Le mot courant a trouver dans la grille
 */
void trouverUnMot(struct Grille* grille, char motCourant[]){
  char motInverse[MAX_DIMENSION];
  strcpy(motInverse, motCourant);
  inverserMot(motInverse);
  for(int y = 0; y < grille->dimension; y++){
    for(int x = 0; x < grille->dimension; x++){
        int longueurMot = strlen(motCourant);
        chercherHorizontal(grille, motCourant, y, x);
        chercherHorizontal(grille, motInverse, y, x);
        chercherVertical(grille, motCourant, y, x);
        chercherVertical(grille, motInverse, y, x);
        chercherDiagonalDroite(grille, motCourant, y, x);
        chercherDiagonalDroite(grille, motInverse, y, x);
        chercherDiagonalGauche(grille, motCourant, y, x);
        chercherDiagonalGauche(grille, motInverse, y, x);
    }
  }
}

/**
 * Permet de parcourir chaque mot a trouver du tableau motsATrouver et de chercher
 * ces derniers dans la grille pour inscrire les lettres utilisés.
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver les mots
 * @param motsATrouver Le tableau contenant les mots cachés
 */
void trouverLesMots(struct Grille* grille, const char motsATrouver[]){
  char motCourant[MAX_DIMENSION];
  int position = 0;
  while(motsATrouver[position] != '\0'){
    retirerMotListe(motsATrouver, &position, motCourant);
    trouverUnMot(grille, motCourant);
  }
  
}

/**
 * Trouver chacune des lettres non utilisés pour former le mot résultant
 *  
 * @param grille Pointeur vers la grille ou l'on doit trouver le mot resultant
 * @param motResultant Le tableau pour stocker le mot résultant
 */
void trouverMotResultant(const struct Grille grille, char motResultant[]){
  int position = 0;
  for (int y = 0; y < grille.dimension; y++) {
    for (int x = 0; x < grille.dimension; x++) {
      if(!grille.choisi[x][y]){
        motResultant[position] = grille.lettres[x][y];
        position++;
      }
    }
  }
}

int main(void) {
  struct Grille grille;
  char lettresGrille[4096];
  char ligne[4096];
  char motsATrouver[4096];
  int lettreCourante = 0;

  while (fgets(ligne, sizeof(ligne), stdin)) {
    strcat(lettresGrille, ligne);
  }

  grille.dimension = trouverDimension(lettresGrille);
  remplirGrille(&grille, lettresGrille, &lettreCourante);
  stockerMotCache(lettresGrille, &lettreCourante, motsATrouver);
  trouverLesMots(&grille, motsATrouver);

  char motResultant[100];
  trouverMotResultant(grille, motResultant);

  printf("%s\n", motResultant);
  
  return 0;
}