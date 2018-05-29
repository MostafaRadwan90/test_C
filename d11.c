# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdbool.h>
bool load(const char* dictionary);
bool check(const char* text);


#define LENGTH 45

int x,hash=0,hasho=0;
         typedef struct node 
           { 
             char word [LENGTH+1 ];
             struct node* next; 
           }node; 
           
            node* hashtabel[901]; 
                           node* checker ; 
 
            
int main (int argc, char* argv[])
{
  char* dictionary = argv[1];
   char* text = argv[2] ;

  load(dictionary);
  printf ("\n hey load is okay foo ");
             /*
              node* checker ; 
              for (int i=0 ; i<1000 ; i++){
              checker= hashtabel[i];
              while (checker!=NULL) {
                  printf ("  %s",checker->word);
                  checker=checker->next;
                                         }
                 printf ("\n %d \n",i);
              }    */
              
    check(text);
printf ("\n hey i come to main agin ");
    
              
  return 0;
}

bool load(const char* dictionary)
{
        // TODO 
        int wordlen,scannedo=0;
        for (int i=0; i<901 ; i++){
                   hashtabel[i]=NULL ;}
        
          // node* newword = malloc (sizeof(node)); 
           FILE* dic = fopen(dictionary, "r");
           if (dic == NULL)
           {
               printf("Could not open %s.\n", dictionary);
               return false;
           }
    for (int i=0; i<120 ; i++)
    {
          node* temp =  malloc (sizeof (node));
          x= fscanf (dic, "%s  ",temp->word);
           scannedo++ ; 
              if (x==1)
              {
                  //printf ("word scanned %s ",temp->word);
                  // printf (" learn how to hash %d ",hashtabel[i]->word[0]);
                   wordlen=strlen(temp->word);
                  hash=(wordlen*temp->word[0]*temp->word[wordlen-1])%886;
                   //printf(" strlen %d and counter is %d \t hash is %d ",wordlen,i,hash);

                      if (hashtabel[hash]==NULL )
                    {
                           hashtabel[hash] =  malloc (sizeof (node));
                           strcpy (hashtabel[hash]->word,temp->word);
                              //printf ("check after copy  %s\n",hashtabel[hash]->word);
                     }
                         else 
                        {
                         node* ptr = malloc (sizeof (node));
                         strcpy (ptr->word,temp->word);
                         ptr->next=hashtabel[hash];
                                                  //printf ("hash is double %s %s \n",ptr->word,hashtabel[hash]->word);

                         hashtabel[hash]=ptr;
                         }
                    }
                    
                    

           
          printf(" strlen %d and counter is %d \t hash is %d \n",wordlen,i,hash);
           // fgetc(dic);
           
    }
        printf ("\n \n \n  total words is  %d \n \n \n ",scannedo);
    fclose (dic);

    return true;
}



bool check(const char* text)
{
FILE* fp = fopen(text, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", text);
        return false;
    }

    // prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH+1];
   // char temp[LENGTH+1];

    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(fp)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(fp)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
            word[index] = '\0';
            // update counter
            words++;
            // strcpy (temp,word);
            int   wordleno=strlen(word);
             printf (" temp is %s ",word) ;

                for(int i = 0; i<=wordleno-1; i++){
                     word[i] = tolower(word[i]);
                         }
               hasho=(wordleno*(word[0])*word[wordleno-1])%886; 
              printf (" word is %s hash is %d",word,hasho) ;

            checker= hashtabel[hasho];
            // print word if misspelled
           while (strcmp(checker->word, word) != 0  )
           
            {
             checker=checker->next;
                 if(checker==NULL){
                    printf("  wrong word is %s \n", word);
                misspellings++;
                 printf ("%d   %d    words scanned %d",index,misspellings,words);

                     goto EXIT;
                }
            }
                EXIT:
                
            // prepare for next word
            index = 0;
        }
    }

    fclose (fp);
return true ; 
}

/* if ( (int)temp[0]>=65 &&(int)temp[0] <=90)
              {
                if ( (int)temp[wordleno-1]>=65 &&(int)temp[wordleno-1] <=90) 
                   {
                     hasho=(wordleno*(temp[0]+32)*(temp[wordleno-1]+32))%1000;
                     temp[0]=temp[0]+32;
                     temp[wordleno-1]=temp[wordleno-1]+32;
                   }
                else
                {
             hasho=(wordleno*(temp[0]+32)*temp[wordleno-1])%1000;
             temp[0]=temp[0]+32;
                }
              }
            else 
                {
               hasho=(wordleno*(temp[0])*temp[wordleno-1])%1000; 
                }  */
