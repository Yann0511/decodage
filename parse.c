#include"decode.h"

struct header *parseheaders(char *buf, int nbuf, int *pos, struct Time_Step_ *time, int *type, char **argv)
{
  struct header *h;
  int nc, nl, i, j, c, k = 0;
  char copie[10], *freq = 0, *date = 0, m_date[7];

  *type = 2;
     
  for(i = *pos; i < nbuf; i++)
    {
      /* Recherche la ligne contenenant le nom de la table, la date et la fréquence d'en registrement. */
      /* Recupereration de la date */
      while(strncmp(buf+i, "\"Table\"", 5))
	i++;
      
      while(buf[i] != ',')
	i--;
      
      i++;
      if(buf[i] == '\"')
	{
	  j = 0;
	  
	  while(buf[++i] != '\"')
	    j++;
	  
	  date = malloc(sizeof(char)*j);
	  strncpy(date, buf+i-j, j);

	  k = j = 0;
	  while(date[k] != '-')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->annee = atoi(m_date);

	  k++;
	  j = 0;
	  while(date[k] != '-')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->mois = atoi(m_date);
	  
	  k++;
	  j = 0;
	  while(date[k] != ' ')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->jour = atoi(m_date);

	   k++;
	  j = 0;
	  while(date[k] != ':')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->heure = atoi(m_date);

	   k++;
	  j = 0;
	  while(date[k] != ':')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->minute = atoi(m_date);

	   k++;
	  j = 0;
	  while(date[k] != '\0')
	    {
	      m_date[j] = date[k];
	      j++;
	      k++;
	    }
	  m_date[j] = '\0';
	  time->seconde = atoi(m_date);

	  time->tiece = 0;
	}

      /* Recupereration de la frequence et de son unité */
      while(strncmp(buf+i, "\"Table\"", 5))
	i++;
      
      while(buf[i] != ',')
	i++;
      
      i++;
      if(buf[i] == '\"')
	{
	  j = 0;
	  k = 0;
	  while(buf[++i] != ' ')
	    k++;
	  
	  while(buf[++i] != '\"')
	    j++;
	  
	  freq = malloc(sizeof(char)*k);
	  strncpy(freq, buf+i-k-j-1, k);
	  time->units = malloc(sizeof(char)*j);
	  strncpy(time->units, buf+i-j, j);

	  time->freq = atoi(freq);
	}      
      
      /* Retrouvez la ligne contenant le nom des colonnes */
      while(*type == 2 )
	{
	  if(strncmp(buf+i, "\"StationID\"", 11) && strncmp(buf+i, "\"Ux\"", 4))
	    i++;
	  else if(!strncmp(buf+i, "\"StationID\"", 11))
	    *type = 1;
	  else if(!strncmp(buf+i, "\"Ux\"", 4))
	    *type = 0;
	}

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
	      h[c].file = malloc(sizeof(char)*(j+strlen(argv[2])+4));
	      strncpy(h[c].file, argv[2], strlen(argv[2]));
	      strncat(h[c].file, buf+i-j, j) ;
	      strcat(h[c].file, ".txt");
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

void increment_date(struct Time_Step_ *time, int freq, char *date)
{

  if(!strncmp(time->units, "USEC", 4))
    {
      freq = 5;
      time->tiece += freq;
      
      if(time->tiece >= 100)
	{
	  time->tiece %= 60;
	  time->seconde++;
	  if(time->seconde >= 60)
	    {
	      time->seconde %= 60;
	       time->minute += freq;
     
	       if(time->minute >= 60)
		 {
		   time->minute %= 60;
		   time->heure++;
		   
		   if(time->heure >= 24)
		     {
		       time->heure %= 24;
		       time->jour++;
		       
		       if(time->mois == 2)
			 {
			   if(time->jour > 28)
			     {
			       time->jour %= 28;
			       time->mois++;
			       
			       if(time->mois > 12)
				 {
				   time->mois %= 12;
				   time->annee++;
				 }
			     }
			 }
		       
		       if(time->mois == 1 || time->mois == 3 || time->mois == 5 || time->mois == 7 || time->mois == 8 || time->mois == 10 || time->mois == 12)
			 {
			   if(time->jour > 31)
			     {
			       time->jour %= 31;
			       time->mois++;
			       
			       if(time->mois > 12)
				 {
				   time->mois %= 12;
				   time->annee++;
				 }
			     }
			 }
		       
		       if(time->mois == 2 || time->mois == 4 || time->mois == 6 || time->mois == 9 || time->mois == 11)
			 {
			   if(time->jour > 30)
			     {
			       time->jour %= 30;
			       time->mois++;
			       
			       if(time->mois > 12)
				 {
				   time->mois %= 12;
				   time->annee++;
				 }
			     }
			 }
		     }
		 }  
	    }
	}
    }
  
  if(!strncmp(time->units, "MIN", 3))
    {
      time->minute += freq;
     
      if(time->minute >= 60)
	{
	  time->minute %= 60;
	  time->heure++;
	   
	  if(time->heure >= 24)
	    {
	      time->heure %= 24;
	      time->jour++;
	       
	      if(time->mois == 2)
		{
		  if(time->jour > 28)
		    {
		      time->jour %= 28;
		      time->mois++;
		  
		      if(time->mois > 12)
			{
			  time->mois %= 12;
			  time->annee++;
			}
		    }
		}
	       
	      if(time->mois == 1 || time->mois == 3 || time->mois == 5 || time->mois == 7 || time->mois == 8 || time->mois == 10 || time->mois == 12)
		{
		  if(time->jour > 31)
		    {
		      time->jour %= 31;
		      time->mois++;
		  
		      if(time->mois > 12)
			{
			  time->mois %= 12;
			  time->annee++;
			}
		    }
		}

	      if(time->mois == 2 || time->mois == 4 || time->mois == 6 || time->mois == 9 || time->mois == 11)
		{
		  if(time->jour > 30)
		    {
		      time->jour %= 30;
		      time->mois++;
		  
		      if(time->mois > 12)
			{
			  time->mois %= 12;
			  time->annee++;
			}
		    }
		}
	    }
	}
    }
    sprintf(date, "%d-%2d-%d %2d:%2d:%2d.%2d", time->annee, time->mois, time->jour, time->heure, time->minute, time->seconde, time->tiece);
    return;
}

void arrondi_date_(struct Time_Step_ *time)
{
	time->seconde = 0;

	if(time->minute < 60 && time->minute > 50 && time->freq == 1)
	  increment_date(time, time->freq, NULL);

	else if(time->minute > 1 && time->minute < 40)
		time->minute -= time->minute%10;
	if(time->minute < 60 && time->minute > 50 && time->freq == 30)
	{
		time->minute = time->minute + (10 - time->minute%10);
		if(time->minute == 60)
		{
			time->minute--;
			increment_date(time, 1, NULL);
		}
			
	}
		
}
void parsevalues(struct header *h, char *tmp, int *pos, int size, int input, int output, struct Time_Step_ *time, char *date)
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

      if(!strncmp(tmp+i, "Dangbo", 6))
	break;
    }
     
  /* J'enregistre la sequence correspondant à l'id de la station */
  memcpy(seq_id_station, tmp+i, 24);
  seq_id_station[24] = '\0';

  while(i <= size)
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
      if(type_size > size-i-1)
	{
	  lseek(input, -(size-i), 1);
	  size = read(input, tmp, SIZE);
	  i = 0;
	}
	  
      memcpy(seq, tmp+i, type_size);
      seq_decode = decode(seq, h[c].type);

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

      if(strncmp(buf, "Dangbo", 6))
	{
	  output = open(h[c].file , O_WRONLY | O_APPEND | O_CREAT, 0644 );
	  write(output, date, strlen(date));
	  write(output, buf, strlen(buf));
	  write(output, "\n", 1);
	  close(output);
	}

      i = i + type_size;

      /* Quand on atteint la fin du tableau */
      if(i == size)
	{
	  size = read(input , tmp , SIZE) ;
	  i = 0;
	}
	  
      if(h[c+1].name == 0)
	{
	  increment_date(time, time->freq, date);
	       
	  /* Se placé sur l'id de la station avant de recommencé le décodage 
	     pour une nouvelle ligne */
	  /* On vérifie d'abord qu'on peut bien copié 24 octet à parti de la 
	     position actuelle sinon on recharche le tableau */
	  if(24 > size-1-i)
	    {
	      lseek(input, -(size-i), 1);
	      size = read(input, tmp, SIZE);
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
		  lseek(input, -(size-i), 1);
		  size = read(input, tmp, SIZE);
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



void parsevalues_type_0(struct header *h, char *tmp, int *pos, int size, int input, int output, struct Time_Step_ *time, char *date)
{
  int i, c = 0, nc = 0, type_size, ligne_size = 0;
  char seq[25], buf[25], seq_id_station[25];
  infd seq_decode;
  int p = 0, datapos = 0;


  /* Parcours du fichier jusqu'a trouvé le début de la séquence */
  for(i = *pos; i <= size; i++)
    {
      if(i == size)
	{
	  size = read(input , tmp , SIZE) ;
	  i = 0;
	}
      if(!strncmp(tmp+i, "\r\n", 2))
	break;
    }
  
  i+=14;   

  while(i <= size)
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
      if(type_size > size-i-1)
	{
	  lseek(input, -(size-i), 1);
	  size = read(input, tmp, SIZE);
	  i = 0;
	}
	  
      memcpy(seq, tmp+i, type_size);
      seq_decode = decode(seq, h[c].type);

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

      if(strncmp(buf, "Dangbo", 6))
	{
	  output = open(h[c].file , O_WRONLY | O_APPEND | O_CREAT, 0644 );
	  write(output, date, strlen(date));
	  write(output, buf, strlen(buf));
	  write(output, "\n", 1);
	  close(output);
	}
      
      i = i + type_size;

      /* Quand on atteint la fin du tableau */
      if(i == size)
	{
	  size = read(input , tmp , SIZE) ;
	  i = 0;
	}
	  
      if(h[c+1].name == 0)
	{
	  increment_date(time, time->freq, date);
	  
	  /* Se déplacer de 16 octects avant de recommencé le décodage 
	     pour une nouvelle ligne */
	  /* On vérifie d'abord qu'on peut bien copié 16 octet à parti de la 
	     position actuelle sinon on recharche le tableau */
	  if(16 > size-1-i)
	    {
	      lseek(input, -(size-i), 1);
	      size = read(input, tmp, SIZE);
	      i = 0;
	    }

	  i+=16;

	  c = 0;
	}
      
      else
	c++;
    }
}
