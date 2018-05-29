# include <stdio.h>
# include <cs50.h>
# include <string.h>
#include <ctype.h> 
 
 int main (int argc, string argv[] ) 
 
 {
   int x =0;
   int n= argc ; 
   if (n>=2)
 {
   string keyword =argv[1];   // keyword  
   int k =strlen (keyword); 
   int shiftvalue[k-1];               // to store crossponding shift value for each letter 
   for (int j=0 ; j<k;j++)
   { 
       if (n!=2 || !isalpha(keyword[j]))
       {    
       printf("write an word as encryption key.\n");
       return 1 ;
       }
   }
   for (int j1=0; j1<k;j1++)
   {
    if ( (int)keyword[j1] >=65 &&(int)keyword[j1] <=90 )     // capital letters 
      {
                        shiftvalue[j1] =(keyword[j1]-65 );
      }
    else if ( (int)keyword[j1] >=97 &&(int)keyword[j1] <=122 )         
      {
                            shiftvalue[j1] =(keyword[j1]-97 );
      }   
      
   } 
   
//  printf ("Give me massage to encyrebt :   ");   // ask for plain text   
  string massage = GetString();
  int m = strlen(massage);
   for (int i=0 ; i<m; i++)
                {
                       x = x%k;
  
                   if ( (int)massage[i] >=65 &&(int)massage[i] <=90 )     // capital letters 
                   {
                       int n1 =((massage[i]-65 )+ shiftvalue[x])%26 +65 ;    //new shifted letter
                      printf ("%c",(char) n1);
                        x++;
                    }
                  else if ( (int)massage[i]>=97 && (int)massage[i] <=122 )
                    {
                 int n2 =((massage[i]-97 )+shiftvalue[x])%26 +97 ; 
                 printf ("%c",(char)n2);
                    x++;
                    }
                    
                    else 
                    {
                        printf("%c",massage[i]);
                        
                    }
    
                 }
          
           
           printf("\n");
    return 0;  
 }
 else 
 {  printf("write an word as encryption key.\n");
       return 1  ;
     
 }   // for while loop   
 }              
               
               
          


