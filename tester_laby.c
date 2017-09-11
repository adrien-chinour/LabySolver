#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "laby.c"

int main(int argc, char * argv[]){

   FILE *fp;
  if (argc == 2) {
    fp = fopen(argv[1],"r"); // read mode
    if( fp == NULL ){
      fprintf(stderr,"Problem opening file\n");
      exit(EXIT_FAILURE);
    }
  }
  else {
    fprintf(stderr,"Invalid number of argument\n");
    exit(EXIT_FAILURE);
  }
    

  labyMap* map=getLabyMapFromFile(fp);
  if( map == NULL ){
    fprintf(stderr,"Invalid file\n");
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
