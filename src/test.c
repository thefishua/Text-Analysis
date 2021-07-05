// Tester for ADT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dict.h"
#include "WFreq.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF" // Start of a book
#define ENDING   "***" // End of the book where "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

// Helper Functions
char *lowerCase(char *c);
void OpenData(FILE *in, char *filename, char line[MAXLINE], char word[MAXWORD]);

int main( int argc, char *argv[])
{
   // TODO ... add any variables you need

   FILE  *in;         // currently open file
   Dict   stopwords;  // dictionary of stopwords
   Dict   wfreqs;     // dictionary of words from book
   WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE];  // current input line
   char   word[MAXWORD];  // current word

   // process command-line args
   switch (argc) {
   case 2:
      nWords = 10;
      fileName = argv[1];
      OpenData(in, fileName, line, word);
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      OpenData(in, fileName, line, word);
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }
}
// Opens the Data File 
void OpenData(FILE *in, char *fileName, char line[MAXLINE], char word[MAXWORD]) 
{
    in = fopen(fileName, "r");
    if (in == NULL) {
        fprintf(stderr, "Can't open %s\n",fileName);
    } else {
        while (fgets(line, MAXLINE, in) != NULL) {
            if (strncmp(STARTING, line, strlen(STARTING)) == 0) {
                while (fscanf(in, "%s", word) != EOF && strncmp(ENDING, word, strlen(ENDING)) != 0) {
                    printf("Word is: %s\n", lowerCase(word));
                }
                fclose(in);
            }
        }
    }
}
// Opens the Stopwords file
void OpenStopwords(FILE *in, char word[MAXWORD]) 
{
    in = fopen("stopwords", "r");
    if (in == NULL) {
        fprintf(stderr, "Can't open stopwords\n");
    } else {
        while (fgets(word, MAXWORD, in) != NULL && strncmp(ENDING, word, strlen(ENDING)) != 0) {
        printf("Line is: %s", lowerCase(word));
        fclose(in);
        }
    }
}
// Turns all words in a file to lower case 
// Returns a lowercase word
char *lowerCase(char *c) 
{
    for(int i = 0; c[i] != '\0'; i++) {
        if(c[i] >= 'A' && c[i] <= 'Z')
        {
            c[i] = c[i] + 32;
        }
    }
    return c;
}