#include <stdio.h>
#include <stdlib.h>


int main (int argc, char* argv[])

{

//   argv[1] like example.txt  
  char* foo=argv[1] ;

FILE *f = fopen('foo', 'r');

if (f==NULL)
{
printf(" mistake happen cannot read") ;
return 1;
 }

int c;
while((c = fgetc(f)) != EOF)
{
    printf("next char: '%c' ",c);
}
return 0;
}
