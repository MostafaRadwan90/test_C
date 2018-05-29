#include <stdio.h>
#include <stdlib.h>

int main()
{
int count=0,sum;
for(int i=0;i<10000;i++)
{
sum=0;
 for(int j=i;j<10000;j++)
 {
 sum=sum+j;
 if(sum==10000)
  {
    printf("hey i found matched seq \n");
    for(int x=i;x<=j;x++){
    printf(" %d   ",x);
    count++;}
  }
 }

}

    printf("\n count is %d ",count);
    return 0;
}
