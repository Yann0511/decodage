#include"decode.h"

int main(int argc , char **argv)
{
     int input, output, size, i, j = 0, pos = 0 ;
     char tmp[10240];
     struct header *h = NULL;

     if(argc != 3)
     {
	  printf(" \n Usage : decode < nom du fichier> <nom du fichier de sorti> < station>\n") ;
	  return 84;
     }

     input  = open(argv[1] , O_RDONLY) ;
     output = open(argv[2] , O_CREAT | O_WRONLY , 0644 ) ;
		  
     if(input == -1 || output == -1)
	  return 84 ;

     size = read(input , tmp , 10240) ;
     
     if(!size)
	  return 84 ;
     
     h = parseheaders(tmp, size, &pos) ;

     for(i = 0 ; h[i].name != NULL ; i++)
	  printf("\n%d- %s : %s->%s(%d) , ",++j, h[i].name, h[i].unity, h[i].processing, h[i].type) ;

     parsevalues(h, tmp, size, input, output);
     
     free(h) ;
     close(input) ;
     close(output) ;
    
     return 0 ;
}

	
    
