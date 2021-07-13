#include"decode.h"

int main(int argc , char **argv)
{
  int input, output, size, i, j = 0, pos = 0 ;
  char tmp[SIZE], buf[20], date[40];
  struct header *h = NULL;
  struct Time_Step_ *time = calloc(1, sizeof(struct Time_Step_));
  if(argc != 3)
    {
      printf(" \n Usage : decode < nom du fichier> <nom du fichier de sorti> < station>\n") ;
      return 84;
    }

  input  = open(argv[1] , O_RDONLY) ;
  output = open(argv[2] , O_CREAT | O_WRONLY , 0644 ) ;
		  
  if(input == -1 || output == -1)
    return 84 ;

  size = read(input , tmp , SIZE) ;
     
  if(!size)
    return 84 ;

  /* Recupération de l'entete */
     
  h = parseheaders(tmp, size, &pos, time) ;

  /* Ecrire l'entete dans le output */

  for(i = 0; h[i].name != NULL; i++)
    {
      strcpy(buf, h[i].name);
      strcat(buf, "\t\0");
      if(write(output, buf, strlen(buf)) != strlen(buf))
	printf("\n Erreur d'écriture des noms des colones");
    }

  write(output, "\n", 1);

  for(i = 0; h[i].name != NULL; i++)
    {
      if(h[i].unity)
	{ 
	  strcpy(buf, h[i].unity);
	  strcat(buf, "\t\0");
	  if(write(output, buf, strlen(buf)) != strlen(buf))
	    printf("\n Erreur d'écriture des unités des colones");
	}
    }

  write(output, "\n", 1);

  for(i = 0; h[i].name != NULL; i++)
    {
      if(h[i].processing)
	{
	  strcpy(buf, h[i].processing);
	  strcat(buf, "\t\0");
	  if(write(output, buf, strlen(buf)) != strlen(buf))
	    printf("\n Erreur d'écriture des traitement effectué sur chaque colones");
	}
    }
     
  write(output, "\n", 1);
  sprintf(date, "%d-%d-%d %d:%d:%d, ", time->annee, time->mois, time->jour, time->heure, time->minute, time->seconde);
  write(output, date, strlen(date));
  
  parsevalues(h, tmp, &pos, size, input, output, time);
     
  free(h) ;
  close(input) ;
  close(output) ;
    
  return 0 ;
}
