#ifndef DECODE
#define DECODE
#define SIZE 4098

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<endian.h>
#include<stdbool.h>

struct Time_Step_
{
  int jour;
  int mois;
  int annee;
  int heure;
  int minute;
  int seconde;
  int tiece;
  int freq;
  char *units;
};

struct header
{
  char *name;
  char *unity;
  char *processing;
  int type;
};

enum
  {
   INT4,
   IEEE4,
   IEEE8,
   INT2,
   UINT2,
   UINT4,
   FP2,
   ASCII24,
  };

typedef union 
{
  int in;
  float f;
  double d;
  char id[24];
  short sin;
  long long int lin;
}infd;

infd decode(char *seq, int type);

struct header *parseheaders(char *buf, int nbuf, int*, struct Time_Step_*);

void parsevalues(struct header*, char *tmp, int *pos, int size, int input, int output, struct Time_Step_*);

#endif
