#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define MAX_WIDTH 40
// The usage string
#define USAGE "MOTZKIN MANUAL\n\
NAME\n\
    ./motzkin - draw a Motzkin path\n\
\n\
SYNOPSIS\n\
    ./motzkin STEPS\n\
\n\
DESCRIPTION\n\
    Draws the Motzkin path described by STEPS\n\
    The allowed steps are\n\
      r or R for a (1,0) step\n\
      u or U for a (1,1) step\n\
      d or D for a (1,-1) step\n"

// A Motzkin step
enum step {
  // Moving right (east)
  RIGHT,
  // Moving up (north-east)
  UP,
  // Moving down (south-east)
  DOWN
};

// A Motzkin path
struct motzkin_path {
  // The width of the Motzkin path
  unsigned int width;
  // The height of the Motzkin path
  unsigned int height;
  // The steps
  enum step steps[MAX_WIDTH];
};

/**
 * Vérifie si les lettres entrées pour les steps sont valides,
 * c'est-à-dire u, d ou r sans se soucier de la case, sinon envoie un 
 * message d'erreur significatif.
 *
 * @param lettres Les lettres qui composent les steps.
 * @return true si les lettres sont valides, sinon false.
 */
bool verifierLettresValide(const char lettres[]){
  for(int i = 0; lettres[i] != '\0'; i++){
    if(lettres[i] != 'u' && lettres[i] != 'U' &&
       lettres[i] != 'd' && lettres[i] != 'D' &&
       lettres[i] != 'r' && lettres[i] != 'R'){
        fprintf(stderr, "error: unrecognized step %c\n\n", lettres[i]);
        return false;
       }
  }
  return true;
}

/**
 * Vérifie si les arguments sont valide, c'est-à-dire seulement un
 * argument accepté de grandeur 40 maximal ainsi que les lettres
 * soient valides. Sinon, envoyer un message d'erreur significatif.
 *
 * @param argc Le nombre d'arguments entrés.
 * @param argv Les arguments entrés.
 * @return true si les arguments sont valides, sinon false.
 */
bool verifierArguments(int argc, char* argv[]){
  if(argc != 2) {
    fprintf(stderr, "error: wrong number of arguments\n\n");
    fprintf(stderr,"%s", USAGE);
    return false;
  }
  if(strlen(argv[1]) > 40){
    fprintf(stderr, "error: the path cannot be wider than 40\n\n");
    fprintf(stderr,"%s", USAGE);
    return false;
  }
  if(!verifierLettresValide(argv[1])){
    fprintf(stderr, "%s", USAGE);
    return false;
  }
  return true;
}

/**
 * Crée le chemin motzkin en ajoutant sa largeur, sa hauteur maximal et
 * chaque step par step. Si la hauteur descends sous 0, retourner false
 * et afficher un message significatif. Si la hauteur n'est pas à 0 à la fin
 * du chemin, retourner false et afficher un message significatif.
 *
 * @param motzkin Pointeur vers le chemin motzkin.
 * @param lettres Tableau de caractères représentant chaque step.
 * @return true si le chemin est valide, false sinon.
 */
bool creerCheminMotzkin(struct motzkin_path *motzkin, const char lettres[]){
  motzkin->width = strlen(lettres);
  int hauteurCourante = 0;
  motzkin->height = hauteurCourante;
  
  for(int i = 0; i < motzkin->width; i++){
          switch(lettres[i]){
            case 'r':
            case 'R':
              motzkin->steps[i] = RIGHT;
              break;
            case 'u':
            case 'U':
              motzkin->steps[i] = UP;
              hauteurCourante++;
              motzkin->height = (motzkin->height < hauteurCourante) ? hauteurCourante : motzkin->height;
              break;
            case 'd':
            case 'D':
              motzkin->steps[i] = DOWN;
              hauteurCourante--;
              break;
          }
          if(hauteurCourante < 0){
            fprintf(stderr, "error: the path cannot go below the horizontal axis\n");
            return false;
          }
        }
        if(hauteurCourante != 0){
          fprintf(stderr, "error: the path cannot end above the horizontal axis\n");
          return false;
        }
        return true;
}

/**
 * Crée la grille du chemin motzkin selon ses attributs et l'écrire dans
 * le fichier SVG.
 *
 * @param motzkin Le chemin motzkin auquel la grille doit être créée.
 * @param fichierSVG Pointeur vers le fichier SVG.
 */
void ecrireGrid(const struct motzkin_path motzkin, FILE *fichierSVG){
  fprintf(fichierSVG, "  <g id=\"grid\">\n");
  for(int i = 0; i <= motzkin.width; i++){
    unsigned int x2 = 100*i;
    fprintf(fichierSVG, "    <line x1=\"%d\" y1=\"0\" x2=\"%d\" y2=\"%d\" stroke=\"black\" stroke-width=\"1\" stroke-dasharray=\"4 2\"/>\n", x2, x2, motzkin.height*100);
  }

  for(int i = 0; i <= motzkin.height; i++){
    unsigned int y2 = 100*i;
    fprintf(fichierSVG, "    <line x1=\"0\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" stroke-width=\"1\" stroke-dasharray=\"4 2\"/>\n", y2, motzkin.width*100, y2);
  }
  fprintf(fichierSVG, "  </g>\n");
}

/**
 * Écrire dans le fichier SVG chaque steps du chemin motzkin.
 *
 * @param motzkin Le chemin motzkin auquel les steps doivent être écrit.
 * @param fichierSVG Pointeur vers le fichier SVG.
 */
void ecrireSteps(const struct motzkin_path motzkin, FILE *fichierSVG){
  fprintf(fichierSVG, "  <g id=\"steps\">\n");
  unsigned int x1 = 0;
  unsigned int y1 = 100 * motzkin.height;
  unsigned int x2 = 0;
  unsigned int y2 = y1;
  
  for(int i = 0; i < motzkin.width; i++){
    switch(motzkin.steps[i]){
      case UP:
        y2-= 100;
        break;
      case DOWN:
        y2 += 100;
        break;
    }
    x2 += 100;
    fprintf(fichierSVG, "    <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"red\" stroke-width=\"3\" stroke-dasharray=\"none\"/>\n", x1, y1, x2, y2);
    y1 = y2;
    x1 = x2;
  }
  fprintf(fichierSVG, "  </g>\n");
}

/**
 * Écrire dans le fichier SVG chaque points du chemin motzkin.
 *
 * @param motzkin Le chemin motzkin auquel les points doivent être écrit.
 * @param fichierSVG Pointeur vers le fichier SVG.
 */
void ecrirePoints(const struct motzkin_path motzkin, FILE *fichierSVG){
  fprintf(fichierSVG, "  <g id=\"points\">\n");
  unsigned int cx = 0;
  unsigned int cy = 100 * motzkin.height;
  fprintf(fichierSVG, "    <circle cx=\"%d\" cy=\"%d\" r=\"7\" fill=\"blue\"/>\n", cx, cy);

  for(int i = 0; i < motzkin.width; i++){
    cx += 100;
    switch(motzkin.steps[i]){
      case UP:
        cy-= 100;
        break;
      case DOWN:
        cy += 100;
        break;
    }
    fprintf(fichierSVG, "    <circle cx=\"%d\" cy=\"%d\" r=\"7\" fill=\"blue\"/>\n", cx, cy);
  }
  fprintf(fichierSVG, "  </g>\n");
}

/**
 * Écrire dans le fichier SVG tous le chemins Motzkin et les dimensions
 * de la vue.
 *
 * @param motzkin Le chemin motzkin à écrire dans le fichier.
 * @param fichierSVG Pointeur vers le fichier SVG.
 */
void ecrireFichierSVG(const struct motzkin_path motzkin, FILE *fichierSVG){
  fprintf(fichierSVG, "<?xml version=\"1.0\"?>\n");
  fprintf(fichierSVG, "<svg width=\"%d\" height=\"%d\" viewBox=\"-8 -8 %d %d\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n",
          100*motzkin.width, 100*motzkin.height, 103*motzkin.width, 108*motzkin.height);

  ecrireGrid(motzkin, fichierSVG);
  ecrireSteps(motzkin, fichierSVG);
  ecrirePoints(motzkin, fichierSVG);

  fprintf(fichierSVG, "</svg>\n");
  fclose(fichierSVG);
}

int main(int argc, char* argv[]) {
  if(verifierArguments(argc, argv)){
    FILE *fichierSVG = fopen("chemin_motzkin.svg", "w");

    if(fichierSVG != NULL){
      struct motzkin_path motzkin;

      if(creerCheminMotzkin(&motzkin, argv[1])){
        ecrireFichierSVG(motzkin, fichierSVG);
      } else {
        fprintf(stderr, "%s\n", USAGE);
        return 1;
      }

    } else {
      fprintf(stderr, "error: cannot open file\n");
      fclose(fichierSVG);
      return 1;
    }
    return 0;
  } else {
    return 1;
  }
}
