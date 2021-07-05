// COMP2521 20T2 Assignment 1
// Dict.c ... implementsation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"


typedef struct _DictNode *Link;

typedef struct  _DictNode {
   WFreq  data;
   Link   left;
   Link   right;
	int height;
} DictNode;

struct _DictRep {
   Link tree;
};

// Defintion of static Functions used

static
Link newNode(char *w);

static
Link inDict(Link, char *);

static
Link insertAVL(Link, char *);

static
Link rotateRight(Link);

static
Link rotateLeft(Link);

static
int getHeight(Link);

static
int max(int, int);

static
void preorderTraversal(Link);

static
void fillTopN(Link, WFreq *, int);


// create a new empty Dictionary
Dict newDict()
{
	Dict root = malloc(sizeof(struct _DictRep));
	assert(root != NULL);
	root->tree = NULL;

   return root;
}

// create a new empty node
static
Link newNode(char *w)
{
	Link new = malloc(sizeof(*new));
   if (new == NULL) {
        fprintf(stderr, "Insufficient memory for the NODE!\n"); 
        exit(EXIT_FAILURE);
   }
   new->data.freq = 1;
   new->height = 0;
   new->data.word = strdup(w);
   new->right = new->left = NULL;
   return new;
}

// Returns the pointer of the node containing word if found
// NULL otherwise
// Helper function for DictFind
static
Link inDict(Link root, char *w)
{
	assert(w != NULL);

	if(root == NULL)	return NULL;
	Link found = NULL;
	if(strcmp(w, root->data.word) == 0)	return root;
	else if(strcmp(w, root->data.word) > 0)	found = inDict(root->left, w);
	else if(strcmp(w, root->data.word) < 0)	found = inDict(root->right, w);

	return found;
}

// Rotates the Tree Right
static
Link rotateRight(Link n1)
{
	if (n1 == NULL) return NULL;
	Link n2 = n1->left;
	if (n2 == NULL) return n1;
	n1->left = n2->right;
	n2->right = n1;
	// updating heights here
	n2->height = max(getHeight(n2->left), getHeight(n2->right)) + 1;
	n2->right->height = max(getHeight(n2->right->left), getHeight(n2->right->right)) + 1;

	return n2;
}

// Rotates the Tree Left
static
Link rotateLeft(Link n2)
{
	if (n2 == NULL) return NULL;
	Link n1 = n2->right;
	if (n1 == NULL) return n2;
	n2->right = n1->left;
	n1->left = n2;
	// updating heights here
	n1->height = max(getHeight(n1->left), getHeight(n1->right)) + 1;
	n1->left->height = max(getHeight(n1->left->left), getHeight(n1->left->right)) + 1;

	return n1;
}

// Returns the height of a given node
// -1 if NULL
static 
int getHeight(Link n)
{
	if( n == NULL )	return -1;
	return n->height;
}

// Returns maximum of two integer types
// Helper function for calculating height of a Tree
static
int max(int a, int b)
{
	return a > b? a : b;
}

// Print tree in preorder traversal i.e. (NLR)
// Helper function for showDict
static
void preorderTraversal(Link root)
{
	if(root == NULL)	return;	
	printf("(%s, %d)\n", root->data.word, root->data.freq);
	preorderTraversal(root->left);
	preorderTraversal(root->right);
}
int Balance(Link t) 
{
    if (t == NULL) return 0;
    return getHeight(t->left) - getHeight(t->right);
}
// Avl tree implementation
static
Link insertAVL(Link root, char *w)
{
	if(root == NULL)
	{
		root = newNode(w);
		return root;
	}
	int balance = Balance(root);
	int cmp = strcmp(w, root->data.word);
	if(cmp > 0)	
	{
		root->left = insertAVL(root->left, w);
		if(balance > 0)
		{
			if(cmp < 0)	root->left = rotateLeft(root->left);
			root = rotateRight(root);
		}	
	}	
	else if(cmp < 0)	
	{		
		root->right = insertAVL(root->right, w);
		if( balance > 0)
		{		
			if(cmp > 0)	root->right = rotateRight(root->right);
			root = rotateLeft(root);
		}
	}
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	return root;
}
// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
	assert(d != NULL && w != NULL);

	Link exist = inDict(d->tree, w);
	if(exist != NULL)
	{
		exist->data.freq++;
		return &(exist->data);
	}
	d->tree = insertAVL(d->tree, w);
	
	return &d->tree->data;	
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   assert(d != NULL && w != NULL);
   return &(inDict(d->tree, w)->data);
}

// fill top N by frequency in WFreq array
// Support func for findTopN

static
void fillTopN(Link root, WFreq *wfs, int n)
{
	if (root == NULL) return;
   
   fillTopN(root->left, wfs, n);

   int j = 0;
   while (root->data.freq <= wfs[j].freq && j < n) {
	   j++;
   }
   for (int i = n - 1; i > j; i--) {
      wfs[i] = wfs[i-1];
   }
   wfs[j] = root->data;

   fillTopN(root->right, wfs, n);
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
	fillTopN(d->tree, wfs, n);
  	return n;
}

// print a dictionary
// Showing Dict in preorder traversal order
void showDict(Dict d)
{
	preorderTraversal(d->tree);	
   return;
}
