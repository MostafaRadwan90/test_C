 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[])
{
    // ensure proper usage
   
    // remember filenames
    char* outfile = argv[1];

    // open input file 
   
    // open output fileapa
    FILE* outptr = fopen(outfile, "w+");
    if (outptr == NULL)
    {
        fclose(outptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    fclose(outptr);
 outptr = fopen(outfile, "r+");
    if (outptr == NULL)
    {
        fclose(outptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

int count=0;    
 while(count<37037)
 {  int i=0;
 char ch= 'A';
 for(i=0; i<26; i++){
     fwrite(&ch, sizeof(char), 1, outptr);
     ch++;
     fwrite("\n", sizeof(char), 1, outptr);
 }
 count++;
 int c = 1, d = 1;
 
   for ( c = 1 ; c <= 32767 ; c++ )
       for ( d = 1 ; d <= 32767 ; d++ )
       {}
 }
    
    fclose(outptr);

    return 0;
}

