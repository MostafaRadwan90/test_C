#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc , char* argv[])
{

char* word=argv[1];
char* ptr=malloc (40);
strcpy(ptr,word);
printf (" %s   %s",ptr,word);
scanf ("%s",word);
int x =strcmp (ptr,word) ; 
printf (" %s   %s   %d",ptr,word,x);




return 0;
}























