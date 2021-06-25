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
	       }
	  }

	  *pos = i ;
	  return h ;
     }
     return 0;
}

void parsevalues(struct header *h, char *tmp, int *pos, int size, int input, int output)
{
     int i, c = 0, nc = 0, type_size, ligne_size = 0;
     char seq[25], buf[25], seq_id_station[25];
     infd seq_decode;
     int p = 0, datapos = 0;

     
/* Parcours du fichier jusqu'a trouvé l'id de la station */
     for(i = *pos; i <= size; i++)
     {
	  if(i == size)
	  {
	       size = read(input , tmp , SIZE) ;
	       i = 0;
	  }

	  if(tmp[i] >= 'A' && tmp[i] <= 'Z')
	       break;
     }
     
     /* J'enregistre la sequence correspondant à l'id de la station */
     memcpy(seq_id_station, tmp+i, 24);
     seq_id_station[24] = '\0';

     datapos = i;

     /* taille d'une ligne */
     int z = 0;
     for(z = 0; h[z].name != 0; z++)
     {
	  switch (h[z].type)
	  {
	  case INT4:
	       type_size = sizeof(int);
	       ligne_size += type_size;
	       break;
	       
	  case INT2:
	       type_size = 2;
	       ligne_size += type_size;
	       break;
	       
	  case UINT4:
	       type_size = sizeof(int);
	       ligne_size += type_size;
	       break;
	       
	  case UINT2:
	       type_size = 2;
	       ligne_size += type_size;
	       break;
	       
	  case IEEE4:
	       type_size = sizeof(float);
	       ligne_size += type_size;
	       break;
	       
	  case IEEE8:
	       type_size = sizeof(double);
	       ligne_size += type_size;
	       break;
	       
	  case ASCII24:
	       type_size = 24;
	       ligne_size += type_size;
	       break;
	       
	  case FP2:
	       type_size = 2;
	       ligne_size += type_size;
	       break;
	       //default :
	  }
     }


     while(i < size)
     {
	  switch (h[c].type)
	  {
	  case INT4:
	       type_size = sizeof(int);
	       
	       break;
	       
	  case INT2:
	       type_size = 2;
	       
	       break;
	       
	  case UINT4:
	       type_size = sizeof(int);
	       
	       break;
	       
	  case UINT2:
	       type_size = 2;
	       
	       break;
	       
	  case IEEE4:
	       type_size = sizeof(float);
	       
	       break;
	       
	  case IEEE8:
	       type_size = sizeof(double);
	       
	       break;
	       
	  case ASCII24:
	       type_size = 24;
	       
	       break;
	       
	  case FP2:
	       type_size = 2;
	       
	       break;
	       //default :
	  }

	  /* Si ce qui reste dans tmp n'a pas la meme taille que la colonne actuelle */
	  if(type_size > size-i)
	  {
	       memcpy(tmp, tmp+i, size-i);
	       size = read(input, tmp+size-i+1, i) + size-i;
	       i = 0;
	  }
	  
	  memcpy(seq, tmp+i, type_size);
	  seq_decode = decode(seq, h[c].type);

	  /*test  */

	  /* if(c == 0) */
	  /* { */
	  /*      int k; */
	  /*      printf("\n===========================================\n"); */
	  /*      for(k = 0; k < 6; k++) */
	  /* 	    printf("  %x", *(tmp+k+datapos+ligne_size*p)); */
	  /*      p++; */
	  /*      printf("%d %d", i, datapos); */
	  /*      printf("\n===========================================\n"); */

	  /* } */

	  /* if(h[c+1].name == 0) */
	  /* { */
	  /*      printf("\n===========================================\n"); */


	  /*      int k; */
	  /*      for(k = 0; k < type_size; k++) */
	  /*      { */
	  /* 	    printf(" tmp = %x seq = %x\n", *(tmp+i+k), *(seq+k)); */
	  /*      } */
	  /*      printf("\n===========================================\n"); */
	  /* } */
	  /* fin test */
	  
	  switch (h[c].type)
	  {
	  case INT4:
	       sprintf(buf, ", %d", seq_decode.in);
	       break;
	       
	  case INT2:
	       sprintf(buf, ", %d", seq_decode.sin); 
	       break;
	       
	  case UINT4:
	       sprintf(buf, ", %d", seq_decode.in);
	       break;
	       
	  case UINT2:
	       sprintf(buf, ", %d", seq_decode.sin);
	       break;
	       
	  case IEEE4:
	       sprintf(buf, ", %f", seq_decode.f);
	       break;
	       
	  case IEEE8:
	       sprintf(buf, ", %lf", seq_decode.d);
	       break;
	       
	  case ASCII24:
	       memcpy(buf, seq_decode.id, 24);
	       buf[24] = 0;
	       break;
	       
	  case FP2:
	       sprintf(buf, ", %f", seq_decode.f);
	       break;
	       //default :
	  }

	  write(output, buf, strlen(buf));

	  /* test */
	  /* { */
	  /*      int j; */
	  /*      for(j = 0; j < type_size; j++) */
	  /* 	    printf("%x, ", tmp[i+j]); */
	  /*      printf("type = %d %s, \n", h[c].type, h[c].name); */
	  /* } */
	  /* fin test */

	  i = i + type_size;

          /* Quand on atteint la fin du tableau */
	  if(i == size)
	  {
	       size = read(input , tmp , SIZE) ;
	       i = 0;
	  }

	  
	  if(h[c+1].name == 0)
	  {
	       write(output, "\n", 1);

	       /* infd test = decode(tmp+i, INT4); */
	       /* printf("buf = %s , decode = %d\n",buf, test.in); */

	       
	       /* Se placé sur l'id de la station avant de recommencé le décodage 
		  pour une nouvelle ligne */
	       /* On vérifie d'abord qu'on peut bien copié 24 octet à parti de la 
		  position actuelle sinon on recharche le tableau */
	       if(24 > size-1-i)
	       {
		    memcpy(tmp, tmp+i, size-i);
		    size = read(input, tmp+size-i+1, i) + size-i;
		    i = 0;
	       }

	       memcpy(seq, tmp+i, 24);
	       while(strncmp(seq, seq_id_station, 24))
	       {
		    i++;
		    
		    /* On vérifie d'abord qu'on peut bien copié 24 octet à parti de la 
		       position actuelle sinon on recharche le tableau */
		    if(24 > size-1-i)
		    {
			 memcpy(tmp, tmp+i, size-i);
			 size = read(input, tmp+size-i+1, i) + size-i;
			 i = 0;
		    }
		    
		    /* On vérifie si on est à la fin du fichier */
		    if(size == 0)
			 return;
		    
		    memcpy(seq, tmp+i, 24);
	       }
	       
	       c = 0;
	  }

	  else
	       c++;
     }
     
}
