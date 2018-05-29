#include "stdio.h"
int fun ()
{
static char x=0;
	char y=0;
	x++;
	y++;
	printf("%d %d\n",y,x);
}

int  main()
{

	fun();
	fun();
	fun();
return 0;
}
