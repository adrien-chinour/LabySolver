#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

//The labyMap structure to be used
typedef struct{
  unsigned int levelId; //identifier of the level
  unsigned int width; //number of cells (counting empty ones)
  int* numcontent; //array of the numerical values in order from top left to down right
  char* opcontent; //array of the operators in order from top left to down right
  unsigned int position; //current position
  int startingScore; //score of the starting position
  int goalScore; //target score
} labyMap;

//Constant values to be used
enum direction {UP, DOWN, RIGHT, LEFT, DIRUNDEFINED};
enum operation {PLUS='+', MINUS='-', MULTIPLY='*', DIVIDE='/', UNDEFINED='?'};

int getActualValue(labyMap * m){
  assert(m != NULL);
  return m->numcontent[m->position];
}


labyMap* getLabyMapFromFile(FILE* f){
  if ( f == NULL) return NULL;

  //allocation de labyMap * m
  labyMap* m = (labyMap*) malloc(sizeof(labyMap));
  if( m == NULL) return NULL;

  //allocation numcontent
  int* numcontent = (int*) malloc(sizeof(int)*4096);
  if ( numcontent == NULL) return NULL;

  //allocation chaine de lecture ligne : s
  char* s = (char*) malloc(sizeof(char)*4096);
  if ( s == NULL) return NULL;

  //allocation opcontent
  char * opcontent = (char*) malloc(sizeof(char)*4096);
  if ( opcontent == NULL) return NULL;

  //Lecture ficher map.txt : f
  for(int i = 1; i < 7; i++){
    fgets(s, 4096, f); // On récupére la ligne sous forme de char* : s
    /*
    * Il faut tester le retour de fgets qui renvoie NULL en cas d'erreur
    */
    int j = 0; char* token; // pour case 3

    switch(i){ //gestion cas par cas en fonction de la ligne
      case 1: //level id
      m->levelId = atoi(s);
      /*
      * atoi c'est le démon car on ne peut pas différencier une erreur de la lecture d'un zéro
      */
      break;

      case 2: // width
      if(atoi(s) % 2 == 0 || atoi(s) < 3){ // test si width incorrect
        free(s); free(opcontent); free(numcontent); free(m);
        return NULL;
      }
      m->width = atoi(s);
      break;

      case 3: //numcontent
      token = strtok(s, " "); // on sépare la chaine par paquet séparé par des espaces
      while( token != NULL ) { // tant qu'il existe des paquet
      numcontent[j] = atoi(token);
      j++;
      token = strtok(NULL, " ");
    }
    if(j != (m->width/2+1) * (m->width/2+1) ){ //test si numcontent est correct
      free(s); free(opcontent); free(numcontent); free(m);
      return NULL;
    }
    m->numcontent = numcontent;
    break;

    case 4: //opcontent
    strcpy(opcontent, s); //copie de la chaine de caractère
    if(strlen(opcontent) != m->width/2*(m->width/2+1) *2 +1 ){ //test si opcontent est correct
      free(s); free(opcontent); free(numcontent); free(m);
      return NULL;
    }
    m->opcontent = opcontent;
    break;

    case 5: //position
    if(atoi(s) > 16 || atoi(s) < 0){ //test si position est correct
      free(s); free(opcontent); free(numcontent); free(m);
      return NULL;
    }
    m->position = atoi(s);
    break;

    case 6:
    m->goalScore = atoi(s);
    break;
  }
}
free(s);
m->startingScore = m->numcontent[m->position]; //actualPosition(m)
return m;
}

void drawLabyMap (labyMap * m){
  assert(m != NULL);

  int c_op = 0;
  int c_num = 0;
  printf("Level %d : %d -> %d : %d\n", m->levelId, m->startingScore, m->goalScore, m->numcontent[m->position]);
  for(int i = 0; i < m->width; i++){
    printf("[");
    for(int j = 0; j < m->width; j++){
      if(i%2 == 0){
        if(j%2 == 0){
          if(c_num == m->position)
          printf("%5d^", m->numcontent[c_num]);
          else
          printf("%5d ", m->numcontent[c_num]);
          c_num++;
        }
        else {
          printf("%5c ", m->opcontent[c_op]);
          c_op++;
        }
      }
      else {
        if(j%2 == 0){
          printf("%5c ", m->opcontent[c_op]);
          c_op++;
        }
        else{
          printf("      ");
        }
      }
    }
    printf("]\n");
  }
}

enum operation getOperation(labyMap *m, enum direction d){
  assert(m != NULL);
  char c; // stock le char de opcontent récupéré
  int indice; //indice de opcontent a calculer

  /* Coordonnées de la position actuelle*/
  int x = ( m->position % (m->width /2 +1) ) *2;
  int y = ( m->position / (m->width /2 +1) ) *2;

  switch(d){
    /*MEMO : nombre d'operant sur ligne impair + nombre d'opérant sur ligne pair + nombre d'opérant sur la ligne*/
    case DOWN:
    if( y >= m->width-1 ) //bordure
    return UNDEFINED;
    else
    indice = (y+1)/2*(m->width/2+1) + ((y+1)/2+1)*((m->width)-(m->width/2+1)) + x/2+1 -1;
    break;

    case RIGHT:
    if( x >= m->width-1 ) //bordure
    return UNDEFINED;
    else
    indice = (y)/2*(m->width/2+1) + (y/2)*((m->width)-(m->width/2+1)) + x/2;
    break;

    case UP:
    if( y <= 0 ) //bordure
    return UNDEFINED;
    else
    indice = (y-1)/2*(m->width/2+1) + ((y-1)/2+1)*((m->width)-(m->width/2+1)) + x/2+1 -1;
    break;

    case LEFT:
    if( x <= 0 ) //bordure
    return UNDEFINED;
    else
    indice = (y)/2*(m->width/2+1) + (y/2)*((m->width)-(m->width/2+1)) + x/2 -1;
    break;

    default: return UNDEFINED; break;
  }
  c = m->opcontent[indice];

  /*
  * Il faut utiliser les constantes définies et non pas leurs valeurs
  */
  switch(c){
    case '+': return PLUS; break;
    case '-': return MINUS; break;
    case '*': return MULTIPLY; break;
    case '/': return DIVIDE; break;
    default : return UNDEFINED; break;
  }
}

bool applyOperation(labyMap * m, enum direction d){
  assert( m != NULL);

  enum operation op = getOperation(m,d); //operateur dans la direction
  int ind_num; //indice numcontent a intervertir
  int ind_op; //indice operateur

  /* Coordonnées de la position actuelle*/
  int x = ( m->position % (m->width /2 +1) ) *2;
  int y = ( m->position / (m->width /2 +1) ) *2;

  switch(d){
    case UP:
    ind_num = m->position - (m->width/2+1);
    ind_op = (y-1)/2*(m->width/2+1) + ((y-1)/2+1)*((m->width)-(m->width/2+1)) + x/2+1 -1;
    break;
    case DOWN:
    ind_num = m->position + (m->width/2+1);
    ind_op = (y+1)/2*(m->width/2+1) + ((y+1)/2+1)*((m->width)-(m->width/2+1)) + x/2+1 -1;
    break;
    case LEFT:
    ind_num = m->position - 1;
    ind_op = (y)/2*(m->width/2+1) + (y/2)*((m->width)-(m->width/2+1)) + x/2 -1;
    break;
    case RIGHT:
    ind_num = m->position + 1;
    ind_op = (y)/2*(m->width/2+1) + (y/2)*((m->width)-(m->width/2+1)) + x/2;
    break;
    default: return false; break;
  }

  switch(op){
    case PLUS:
    m->numcontent[m->position] += m->numcontent[ind_num];
    m->opcontent[ind_op] = MINUS;
    break;
    case MINUS:
    m->numcontent[m->position] -= m->numcontent[ind_num];
    m->opcontent[ind_op] = PLUS;
    break;
    case MULTIPLY:
    m->numcontent[m->position] *= m->numcontent[ind_num];
    m->opcontent[ind_op] = DIVIDE;
    break;
    case DIVIDE:
    if(m->numcontent[m->position] % m->numcontent[ind_num] != 0) return false;
    m->numcontent[m->position]/= m->numcontent[ind_num];
    m->opcontent[ind_op] = MULTIPLY;
    break;
    default: return false; break;
  }

  int tmp = m->numcontent[m->position];
  m->numcontent[m->position] = m->numcontent[ind_num];
  m->numcontent[ind_num] = tmp;
  m->position = ind_num;

  return true;
}

static enum direction reverseDir(enum direction d){
  switch(d){
    /*
    * Il faut utiliser les constantes définies et non pas leurs valeurs
    * Une enum peut démarrer à autre chose que 0
    */
    case 0: return 1;
    case 1: return 0;
    case 2: return 3;
    case 3: return 2;
    default: return 4;
  }
}

static enum direction nextDir(enum direction d){
  switch(d){
    case 0: return 2;
    case 1: return 3;
    case 2: return 1;
    case 3: return 4;
    default: return 4;
  }
}

static labyMap * copyLabyMap(labyMap * m){
  assert(m != NULL);

  //allocation labyMap
  labyMap * m_copy = (labyMap*) malloc(sizeof(labyMap));
  assert(m_copy != NULL);

  //allocation numcontent
  int * numcontent = (int*) malloc(sizeof(int)*4096);
  assert(numcontent != NULL);

  //allocation opcontent
  char * opcontent = (char*) malloc(sizeof(char)*4096);
  assert(opcontent != NULL);

  //copie de m->opcontent et m->numcontent dans opcontent et numcontent
  strcpy(opcontent, m->opcontent);
  for(int i = 0; i < (m->width/2+1) * (m->width/2+1) ; i++){
    numcontent[i] = m->numcontent[i];
  }

  //affectation des valeurs de m dans m_copy
  m_copy->levelId = m->levelId;
  m_copy->width = m->width;
  m_copy->opcontent = opcontent;
  m_copy->numcontent = numcontent;
  m_copy->position = m->position;
  m_copy->startingScore = m->startingScore;
  m_copy->goalScore = m->goalScore;

  return m_copy;
}

enum direction* resolve(labyMap * map){
  //copie de map
  labyMap * m = copyLabyMap(map);

  //creation d'un tableau pour stocker les direction
  enum direction tab_dir[4096];

  //tableau nombre de passage sur chaque case
  int nb_passage[(m->width/2+1) * (m->width/2+1)];
  for(int i = 0; i < (m->width/2+1) * (m->width/2+1); i++){ nb_passage[i] = 0; }

  // k = indice de direction et d = direction
  int k = 0;
  enum direction d = UP;

  // boucle qui récupère le chemin
  while( getActualValue(m) != m->goalScore ){
    if(nb_passage[m->position] <= 2 && tab_dir[k-1] != reverseDir(d) && applyOperation(m,d)){ //direction correct et possible
      tab_dir[k] = d;  // on ajoute la direction dans le tableau
      d = UP; // on reinitialise la direction a tester
      k++; // on avance l'indice du tableau
      nb_passage[m->position]++; // on ajoute un passage sur la case
    }
    else if(nb_passage[m->position] <= 2 && d != DIRUNDEFINED){ //direction incorrect
      d = nextDir(d); // on passe a la direction suivante
    }
    else { //aucun possibilité ; retour position arrière
      d = tab_dir[k-1]; // on récupère la dernière direction prise
      nb_passage[m->position]--; // on enlève un passage a notre position (retour arrière)
      applyOperation(m, reverseDir(d)); // on applique l'opération dans le sens inverse
      tab_dir[k] = DIRUNDEFINED; // on ajoute DIRUNDEFINED a notre case actuelle (pas très utile)
      d = nextDir(tab_dir[k-1]); // direction suivante de celle effectuer précedement (pour pas refaire la même chose)
      k--; // recule de l'indice du tableau
    }
  }

  //ajout de DIRUNDEFINED a la fin du chemin
  tab_dir[k] = DIRUNDEFINED;

  // copie de la solution
  enum direction* solution = (enum direction*) malloc(sizeof(enum direction)*k+1);
  for(int i = 0; i < k+1; i++){
    solution[i] = tab_dir[i];
  }

  //free allocation copie
  free(m->opcontent);
  free(m->numcontent);
  free(m);

  return solution;
}
