# include <stdio.h>
# include <stdlib.h>
#include <time.h> 
int rand(void);


#define  MX 500;
int count=0;
int main ()
{


int x[500];

for (int i=0 ; i<500  ; i++ )
{
  srand(i+4);
int y=rand() ;
 do 
{
srand(time(NULL));
int k=rand() ;
y=(k*y)%500;

}
while(y<0 || y>=500);

printf("%d   \n\n",y);
x[i]=y; 



 if (i>0 && x[i-1]>x[i]);
{
int m ;  
m=x[i-1];
x[i-1]=x[i];
x[i]=m;
}

}

for (int  z=0 ; z<500  ; z++ )
{

 printf (" %d    \n",x[z]) ;

}
return 0 ; 

}

