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

struct Time_Step_{
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
  char *file;
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

void arrondi_date_(struct Time_Step_ *);

void increment_date(struct Time_Step_ *time, int freq, char *date);

struct header *parseheaders(char *buf, int nbuf, int*, struct Time_Step_*, int *type, char **argv);

void parsevalues(struct header*, char *tmp, int *pos, int size, int input, int output, struct Time_Step_*, char *date);

void parsevalues_type_0(struct header *h, char *tmp, int *pos, int size, int input, int output, struct Time_Step_ *time, char *date);
#endif
