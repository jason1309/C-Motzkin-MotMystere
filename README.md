# C-Motzkin-MotMystere
Deux petits projets réalisés lors du cours INF3135 - Construction et maintenance de logiciels.

`hw.c` permet de résoudre un mot mystère. Le programme lit sur `stdin` la grille de lettres, suivie de la lsite des mots cachés et écrit la solution sur `stdout`.

`motzkin.c` prends en argument, sur `stdin`, un mot de Motzkin et génère une image SVG du chemin Motzkin correspondant, stocké dans le fichier [`chemin_motzkin.svg`](./chemin_motzkin.svg).

## hw.c
Pour compilé :
```sh
$ make hw
```

Exemple avec [`fruits-legumes.txt`](examples/fruits-legumes.txt)
```sh
$ cat examples/fruits-legumes.txt
FEUILLUSOTPO
RPRADISPRUNE
AILVBONAAAEM
INPOSURSNHCE
SAECMPETGCTR
NRCAJUSEEIAB
ADHTDREQMTRM
VMELONTUARIO
EETOMATEIANC
TFENOUILSREN
ESIOBMARFREO
CITRONIRELEC
AIL
ARTICHAUT
AVOCAT
BON
CELERI
CITRON
CONCOMBRE
EPINARD
FENOUIL
FEUILLUS
FRAIS
FRAMBOISE
JUS
MAIS
MELON
NAVET
NECTARINE
ORANGE
PASTEQUE
PECHE
PRUNE
PUR
RADIS
SUR
TOMATE
$ cat examples/fruits-legumes.txt | ./hw
POMMEDETERRE
$ echo $?
0
```

Pour effectuer les tests:
```sh
$ git submodule update --init
$ bats hw.bats
```

## motzkin.c
Le mot Motzkin peut être composé des lettres suivantes:
- U ou u pour un déplacement en direction *nord-est*
- D ou d pour un déplacement en direction *sud-est*
- R ou r pour un déplacement en direction *est*

Voici comment compiler et un exemple d'exécution:

```sh
$ make motzkin
$ ./motzkin uudd
$ ./motzkin uruddruudduuruddduuuruuuruudddrddrdrrdd
$ echo $?
0
```

Chaque exécution va écrire dans le fichier `chemin_motzkin.svg` et effacer son ancien contenu.
Par exemple, après avoir effectué ces deux commandes ci-dessus, voici le Motzkin affiché:

![Le chemin de Motzkin `uruddruudduuruddduuuruuuruudddrddrdrrdd`](./chemin_motzkin.svg)

Les cas d'erreurs gérés sont les suivants:
- Aucun argument a été fourni au programme;
- Le mot Motzkin contient une lettre invalide (différente de `u`, `d` et `r`);
- Le chemin Motzkin ne termine pas à `(n,0)`;
- Le chemin Motzkin passe sous le segment `(0,0) - (n,0)`;
- Le chemin est trop long, soit plus de 40 steps.

Le manuel d'exécution est affiché après chaque erreur.

Voici des exemples d'exécutions pour chaque erreur:

```sh
$ ./motzkin; echo $?
error: wrong number of arguments

MOTZKIN MANUAL
NAME
    ./motzkin - draw a Motzkin path

SYNOPSIS
    ./motzkin STEPS

DESCRIPTION
    Draws the Motzkin path described by STEPS
    The allowed steps are
      r or R for a (1,0) step
      u or U for a (1,1) step
      d or D for a (1,-1) step
1
$ ./motzkin uubdd; echo $?
error: unrecognized step b

MOTZKIN MANUAL
NAME
    ./motzkin - draw a Motzkin path

SYNOPSIS
    ./motzkin STEPS

DESCRIPTION
    Draws the Motzkin path described by STEPS
    The allowed steps are
      r or R for a (1,0) step
      u or U for a (1,1) step
      d or D for a (1,-1) step
1
$ ./motzkin uuud; echo $?
error: the path cannot end above the horizontal axis
MOTZKIN MANUAL
NAME
    ./motzkin - draw a Motzkin path

SYNOPSIS
    ./motzkin STEPS

DESCRIPTION
    Draws the Motzkin path described by STEPS
    The allowed steps are
      r or R for a (1,0) step
      u or U for a (1,1) step
      d or D for a (1,-1) step

1
$ ./motzkin uurddd; echo $?
error: the path cannot go below the horizontal axis
MOTZKIN MANUAL
NAME
    ./motzkin - draw a Motzkin path

SYNOPSIS
    ./motzkin STEPS

DESCRIPTION
    Draws the Motzkin path described by STEPS
    The allowed steps are
      r or R for a (1,0) step
      u or U for a (1,1) step
      d or D for a (1,-1) step

1
$ ./motzkin uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuddddddddddddddddddddddddddd; echo $?
error: the path cannot be wider than 40

MOTZKIN MANUAL
NAME
    ./motzkin - draw a Motzkin path

SYNOPSIS
    ./motzkin STEPS

DESCRIPTION
    Draws the Motzkin path described by STEPS
    The allowed steps are
      r or R for a (1,0) step
      u or U for a (1,1) step
      d or D for a (1,-1) step
1
```
