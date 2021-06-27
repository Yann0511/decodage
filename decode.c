#include"decode.h"

float fp2__flat(void *buff_)
{
  char *buff = (char*)buff_;
  short fs_word = (short)(buff[0] << 8) + (short)buff[1];
  bool is_negative = ((fs_word & 0x8000) != 0) ;
  short mantisse = fs_word & 0x1FFF, i ;
  short exposant = (fs_word & 0x6000) >> 13 ;
  float rtn ;

  rtn = (float)mantisse ;

  for(i = 0 ; mantisse != 0 && i < exposant ; i++)
    rtn /= 10.0 ;
  if(is_negative && mantisse != 0)
    rtn *= -1.0 ;
 
  return (float)rtn ;
}


infd decode(char *seq, int type)
{ 
  infd recup;
  
  switch (type)
    {
    case INT4 :
      recup.lin = *((int*)seq);
      recup.lin = be32toh(recup.lin);
      recup.in = *((int*)&recup.lin);
      break;
       
    case INT2:
      recup.lin = *((int*)seq);
      recup.lin = be16toh(recup.lin);
      recup.sin = *((short*)&recup.lin);
      break;
       
    case UINT4:
      recup.lin = *((int*)seq);
      recup.lin = be32toh(recup.lin);
      recup.in = *((int*)&recup.lin);
      break;
       
    case UINT2:
      recup.lin = *((int*)seq);
      recup.lin = be16toh(recup.lin);
      recup.sin = *((short*)&recup.lin);
      break;
       
    case IEEE4:
      recup.lin = *((int*)seq);
      recup.lin = be32toh(recup.lin);
      recup.f = *((float*)&recup.lin);
      break;
       
    case IEEE8:
      recup.lin = *((int*)seq);
      recup.lin = be64toh(recup.lin);
      recup.d = *((double*)&recup.lin);
      break;
	 
    case ASCII24:
      strncpy(recup.id, seq, ASCII24);
      break;
       
    case FP2:
      recup.f = fp2__flat(seq);
      break;
      //default :
    }
  
  return recup;
}
  
#ifdef _TEST_ 
int main(void)
{
  char seq[] = {0x00, 0x00, 0x4,0xb0};
  infd recup;
       
  recup = decode(seq, INT4);
       
  printf("%d, %d ",recup.in, sizeof(short));
  return 0;
       
}
#endif
  
