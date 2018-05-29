#include <stdio.h>
#include <stdlib.h>

int main () 
{

int *x = malloc (sizeof(int)) ; 
*x=10 ;
int y =x ;
printf ("%p     %i  %d    %d  \n ",x,*x,y,&y ); 
free(x) ;

return 0 ;


}

