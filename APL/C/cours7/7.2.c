#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fct(int *pint, char *pchar){

  srand(time(NULL));
  
  *pint=rand() %13  + 1;
  int random2=rand() %4 + 1;

  if(random2==1)
    *pchar='P';
  if(random2==2)
    *pchar='C';
  if(random2==3)
    *pchar='T';
  if(random2==4)
    *pchar='K';

}

void main(){

  int chiffre;
  char couleur;

  fct(&chiffre,&couleur);

  printf("%d %c", chiffre, couleur);
}
