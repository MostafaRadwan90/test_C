#include <stdio.h>
#include <string.h>

int main()
{
    char s[1000];
    int count=0;
    
        gets(s) ;
        count += strlen(s);
    printf("%d        ,%d   \n",count,sizeof(char));
   return 0 ;
 } 

