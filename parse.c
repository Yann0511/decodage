#include"decode.h"

struct header *parseheaders(char *buf, int nbuf, int *pos)
{
     struct header *h;
     int nc, nl, i, j, c, k = 0;
     char copie[10] ;
     
     for(i = *pos; i < nbuf; i++)
     {
	  /*saut de 3 lignes */
	  for(nl = 0; nl != 3; i++)
	       if(buf[i] == '\n')
		    nl++;

	  /*compter le nombre de colonne*/
	  j = i;
	  for(nc = 0; buf[i] != '\n'; i++)
	       if(buf[i] == ',')
		    nc++;
	  nc++;

	  h = calloc(nc+1, sizeof(struct header));

	  /*remplir les nom de chaque colonnes*/
	  for(i = j, c = 0; c < nc; i++)
	  {
	       if(buf[i] == '\"')
	       {
		    j = 0;

		    while(buf[++i] != '\"')
			 j++;
		    
		    h[c].name = malloc(sizeof(char)*j);
		    strncpy(h[c].name, buf+i-j, j) ;
		    c++ ;
	       }
	  }

	  /*remplir les unités de chaque colonne. i+2 car la ligne précedente se termine par \r\n */
	  for(i = i+2, c = 0; c < nc; i++)
	  {
	       if(buf[i] == '\"')
	       {
		    j = 0;
		    
		    while(buf[++i] != '\"')
			 j++;

		    if(!j)
			 c++ ;
		    
		    else
		    {
			 h[c].unity = malloc(sizeof(char)*j);
			 strncpy(h[c].unity, buf+i-j, j) ;
			 c++ ;
		    }
	       }
	  }

	  /*remplir le types de traitement fait sur chaque colonne. i+2 car la ligne précedente se termine par \r\n */
	  for(i = i+2, c = 0; c < nc; i++)
	  {
	       if(buf[i] == '\"')
	       {
		    j = 0;
		    
		    while(buf[++i] != '\"')
			 j++;

		    if(!j)
			 c++ ;
		    
		    else
		    {
			 h[c].processing = malloc(sizeof(char)*j);
			 strncpy(h[c].processing, buf+i-j, j) ;
			 c++ ;
		    }
	       }
	  }

	  /*remplir le types de chaque colonne. i+2 car la ligne précedente finir par \r\n*/
	  for(i = i+2, c = 0; c < nc; i++)
	  {
	       if(buf[i] == '\"')
	       {
		    j = 0;
		    while(buf[++i] != '\"')
			 j++;

		    if(!strncmp(buf+i-j, "IEEE4", 5))
		    {
			 h[c].type = IEEE4 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "INT4", 4))
		    {
			 h[c].type = INT4 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "IEEE8", 5))
		    {
			 h[c].type = IEEE8 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "INT2", 4))
		    {
			 h[c].type = INT2 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "FP2", 3))
		    {
			 h[c].type = FP2 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "ASCII", 5))
		    {
			 h[c].type = ASCII24 ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "Boolean", 7))
		    {
			 h[c].type = Boolean ;
			 c++;
		    }

		    else if(!strncmp(buf+i-j, "BOOL8", 5))
		    {
			 h[c].type = BOOL8 ;
			 c++;
		    }
	       }
	  }

	  h[nc].name = NULL ;
	  *pos = i ;
	  return h ;
     }
}

void parsevalues(struct header *h, char *tmp, int *pos, int size, int input, int output)
{
     int i;
     /* Parcours du fichier jusqu'a trouvé l'id de la station */
     for(i = *pos; i <= size; i++)
     {
	  if(i == size)
	  {
	       size = read(input , tmp , 10240) ;
	       i = 0;
	  }

	  if(tmp[i] >= 'A' && tmp[i] <= 'Z')
	       break;
     }

     printf("\n %c", tmp[i]);
}
