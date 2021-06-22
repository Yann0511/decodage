#ifndef DECODE
#define DECODE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <endian.h>

struct header
{
     char *name;
     char *unity;
     char *processing;
     int type;
};

enum
{
     INT4 = sizeof(int),
     IEEE4 = sizeof(float),
     IEEE8 = sizeof(double),
     INT2 = sizeof(short),
     UINT2 = sizeof(short),
     UINT4 = sizeof(int),
     FP2 = 2,
     ASCII24 = 24,
     Boolean = 1,
     BOOL8 = 8
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

struct header *parseheaders(char *buf, int nbuf, int*);

void parsevalues(struct header*, char *tmp, int size, int in, int out);

#endif
