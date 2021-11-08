#include"decode.h"

int main(int argc , char **argv)
{
  int input, output, size, i, j = 0, pos = 0, *type = calloc(1, sizeof(int)) ;
  char tmp[SIZE], buf[20], *date = calloc(40, sizeof(char));;
  struct header *h = NULL;
  struct Time_Step_ *time = calloc(1, sizeof(struct Time_Step_));
  if(argc != 3)
    {
      printf(" \n Usage : decode < nom du fichier> <nom du fichier de sorti> < station>\n") ;
      return 84;
    }

  input  = open(argv[1] , O_RDONLY) ;
		  
  if(input == -1)
    return 84 ;

  size = read(input , tmp , SIZE) ;
     
  if(!size)
    return 84 ;

  /* Recupération de l'entete */
     
  h = parseheaders(tmp, size, &pos, time, type, argv) ;

  /* Création des fichiers */

  for(i = 0; h[i].name != NULL; i++)
    {
      if(!strncmp(h[i].file, "StationID", 9))
	continue;
      else
	{
	  output = open(h[i].file , O_CREAT | O_WRONLY , 0644 );
	  close(output) ;
	}
    }

  arrondi_date_(time);
  sprintf(date, "%d-%d-%d %2d:%2d:%2d.%2d ", time->annee, time->mois, time->jour, time->heure, time->minute, time->seconde, time->tiece);

  if(*type == 1)
    parsevalues(h, tmp, &pos, size, input, output, time, date);
  else
    parsevalues_type_0(h, tmp, &pos, size, input, output, time, date);
  
  free(h) ;
  free(time);
  free(date);
  close(input) ;
    
  return 0 ;
}
