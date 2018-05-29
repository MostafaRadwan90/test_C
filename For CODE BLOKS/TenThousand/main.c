#include <stdio.h>

/* print_sequence	: Prints a sequence of numbers
 * start			: Sequence start
 * end				: Sequence end
 * Return			: None
 */
void print_sequence(unsigned int start, unsigned int end);

/* find_sequence	: Finds a sequence of numbers that when summed gives x
 * x				: Sum of sequence to be found
 * Return			: None
 */
void find_sequence(unsigned int x);

int main()
{
	int x=10000;

	find_sequence(x);
	return 0;
}

void print_sequence(unsigned int start, unsigned int end)
{
    int i;

	for( i= start;i<=end;i++)
    {
        printf("%d ",i);

    }
    printf("\n\n");
    printf("end of sequence/////////////////////////////////////end of sequence\n");
    printf("\n\n");
}

void find_sequence(unsigned int x)
{
    int start;
    int end;
    int i;
    int j ;
	for( i=1;i<=x;i++)
    {
        int sum=0;
        for( j=i;j<=x;j++)
        {
            sum=sum+j;
            if (sum==x)
            {
                start=i;
                end=j;
                print_sequence(start,end);
            }
            else if(sum<x)
            {
                continue;
            }
            else
                break;
        }
    }

}
