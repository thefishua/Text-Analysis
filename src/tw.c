// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

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

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')
void lexiocographical (WFreq *results, int nWords);

int main( int argc, char *argv[])
{
   // Variables declarations
   Dict stopwordDict = newDict();  	// dictionary of stopwords
	Dict gutenburg = newDict();		// dictionary of words from gutenburg project
//int wordsInserted = 0;
   WFreq *highestFreqW; // could be two words with the same highest freq
   int mostFrequent = 0;

   FILE  *in;         // currently open file
   WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE] = "";  // current input line
   char   word[MAXWORD] = "";  // current word

   // process command-line args
   switch (argc) {
	case 1:
		fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
		exit(EXIT_FAILURE);
   case 2:
      nWords = 10;
      fileName = argv[1];
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // build stopword dictionary
  
	stopwordDict = newDict();
	in = fopen(STOPWORDS, "r");
	if(in == NULL)	
	{	
		fprintf(stderr, "Can't open stopwords\n");
		exit(EXIT_FAILURE);
	}	
	while(fgets(line, MAXWORD, in) != NULL)
	{
		for(int i = 0; i < strlen(line)-1; i++)	word[i] = line[i];
		DictInsert(stopwordDict, word);
		for(int i = 0; i < MAXWORD; i++)	word[i] = '\0';
	}
	fclose(in);

	// SCAN UPTO THE START OF TEXT

	gutenburg = newDict();	
	in = fopen(fileName, "r");
	if(in == NULL)
	{
		fprintf(stderr, "Can't open %s\n", fileName);
		exit(EXIT_FAILURE);
	}

	while(fgets(line, MAXLINE, in) != NULL)
	{
		if(strncmp(line, STARTING, strlen(STARTING)) == 0)	break;
	}
 
	// SCAN FILE READING WORDS AND ACCUMULATING COUNTS
	while (fgets(line, MAXLINE, in) != NULL) { 
      if (line[0] != '\n') {
         if (!strncmp(ENDING, line, strlen(ENDING))) break; // break once it hits *** END OF
         
         int j = 0;
         char ch;
         for (int i = 0; line[i]; i++){ 
            ch = line[i];
            if (isWordChar(ch)) {
               word[j] = tolower(ch);
               j++;
            } else if (!isWordChar(ch)) {
               WFreq *res = DictFind(stopwordDict, word); // returns NULL if its not in the stop words dict
                  if (res == NULL) { // apply stemmer to a non-stopword
                     int index = stem(word, 0, strlen(word)-1); 
                     word[index + 1] = '\0';                     
                     if (strlen(word) > 1) {// add word to dictionary 
                        WFreq *w = DictInsert(gutenburg, word); 
                        if (w->freq > mostFrequent) { // keep track of what the highest number is
                           highestFreqW = w;
                           mostFrequent = w->freq;
                        }
                     }
                  }
                  memset(word, 0, MAXWORD);
                  j = 0;
               }
           
         }    
      }
   }
   results = (WFreq *) malloc((nWords) * sizeof(*results));
   results[0] = *highestFreqW; // first word will be highest
	
	// COMPUTE AND DISPLAY THE TOP N WORDS

	nWords = findTopN(gutenburg, results, nWords);
	for(int i = 1; i < nWords; i++) {
		if (results[i].freq == 0) break;
		printf("%7d %s\n", results[i].freq, results[i].word);
	}

   return EXIT_SUCCESS;
}

void lexiocographical (WFreq *results, int topN) 
{
   char *temp = NULL;
	for (int i = 0; i < topN; ++i) {
      for (int j = i + 1; j < topN; ++j) {
         // swapping strings if they are not in the lexicographical order
         if (strcmp(results[i].word, results[j].word) > 0) {
            strcpy(temp, results[i].word);
            strcpy(results[i].word, results[j].word);
            strcpy(results[j].word, temp);
         }
      }
   }
}
