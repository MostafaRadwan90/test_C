#include <stdio.h>

int main () 
{ 

int count=0;
for (int i=0 ; i<=400 ;i++)
{
for (int j=0 ; j<=400 ;j++)
{
for (int k=0 ; k<=400 ;k++)
{

if (i+k+j==400){
printf (" i found it   %d   %d    %d \n ",i,j,k);
count++;
}
}
}
}
printf("%d     \n",count);
return 0; 

}  

