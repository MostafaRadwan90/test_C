
#include <stdio.h>
void IsRightAngle();
int main()
{
     /*int arr[30];
    printf("%d",sizeof(arr));
    printf("\n");
    return 0; */
    IsRightAngle(3,4,5);
    IsRightAngle(3,8,5);
    IsRightAngle(8,10,6);
    IsRightAngle(1,4,5);
    IsRightAngle(1,0,5);
    return 0;
}

void IsRightAngle(unsigned int a,unsigned int b,unsigned int c)
{
    if(a==0||b==0||c==0)
    {
        printf("wrong input(there is no zero side) \n");
    }
    if(a!=0&&b!=0&&c!=0)
    {
         int t=0;
        if ((a*a==b*b+c*c)||(b*b==a*a+c*c)||(c*c==a*a+b*b))
        {
            t=1;
        }
        if(t==0)
        {
            printf("those sides don't form a right angle \n");
        }
        else if(t==1)
        {
            printf("those sides form a right angle \n");
        }
    }

}

