
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
int main (int argc,char* argv []) 
{

int opt=0;

while (opt= getopt(argc, argv,"hp:")!=-1);
{

if (opt == 'p')
{
printf ("opt is p \n");
}
else if (opt == ':')
{
printf ("opt is : \n");
}
else if (opt == 'h')
{
printf ("opt =h");
}
else
 {
printf ("hey cannot found any thing \n");
}

printf ("opt is %d \n",opt);}
printf ("finalll opt is %d \n",opt);
return 0 ; 
}
