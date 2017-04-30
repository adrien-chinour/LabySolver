#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "laby.c"

int main(void){



  FILE *fp;
  // Il faut changer le nom du fichier pour tester sur une autre map
  fp = fopen("map/map15.txt","r"); // read mode
  if( fp == NULL ){
    fprintf(stderr,"Problem opening file");
    exit(EXIT_FAILURE);
  }

  labyMap* map=getLabyMapFromFile(fp);
  if( map == NULL ){
    fprintf(stderr,"Invalid file");
    exit(EXIT_FAILURE);
  }


  printf("## Map au début :\n");
  drawLabyMap(map);
  printf("\n");

  printf("## Chemin de résolution : \n");
  enum direction * solution = resolve(map);
  bool stop=false;
  while(!stop){
    switch(*(solution)){
      case UP:
      printf("UP\n");
      applyOperation(map, UP);
      break;
      case DOWN:
      printf("DOWN\n");
      applyOperation(map, DOWN);
      break;
      case LEFT:
      printf("LEFT\n");
      applyOperation(map, LEFT);
      break;
      case RIGHT:
      printf("RIGHT\n");
      applyOperation(map, RIGHT);
      break;
      case DIRUNDEFINED:
      stop=true;
    }
    solution++;
  }
  printf("\n");

  printf("## Map résolue :\n");
  drawLabyMap(map);


  free(map->numcontent);
  free(map->opcontent);
  free(map);
  fclose(fp);

  return EXIT_SUCCESS;
}
