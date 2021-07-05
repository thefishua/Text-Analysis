// COMP2521 20T2 Assignment 1
// Dict.c ... implementation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dict.h"
#include "WFreq.h"

typedef struct _DictNode *Link;

typedef struct  _DictNode {
   WFreq  data;
   int height;
   Link   left;
   Link   right;
} DictNode;

struct _DictRep {
   Link tree;
};
// Helper Functions
Link insert(Link t, char *w);
Link search(Link t, char *w);
int height(Link t);
int depth(Dict t);
Link rotateRight(Link t);
Link rotateLeft(Link t);
Link newNode(char *word);
void printTree(Link t);
int max(int a, int b);
void findWordFrequency(Link t, WFreq *wfs, int n);

// create new empty Dictionary
Dict newDict()
{
    Dict new = malloc(sizeof(*new));
    if (new == NULL) {
        fprintf(stderr, "Insufficient memory for the TREE!\n"); 
        exit(EXIT_FAILURE);
    }
    new->tree = NULL;
    return new;
}
// create a new node for the BST tree
Link newNode(char *word)
{
   Link new = malloc(sizeof(*new));
   if (new == NULL) {
        fprintf(stderr, "Insufficient memory for the NODE!\n"); 
        exit(EXIT_FAILURE);
   }
   new->data.freq = 1;
   new->height = 0;
   new->data.word = strdup(word);
   new->right = new->left = NULL;
   return new;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
    Link check = search(d->tree, w);
    if (check != NULL) {
        check->data.freq++;
        return &check->data;
    }
    d->tree = insert(d->tree, w);
    return &d->tree->data;
}

// find Word in Dictionary
// return pointer to the (word,freq) pair if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
    d->tree = search(d->tree, w);
    if(d->tree != NULL) return &d->tree->data;
    return NULL;
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
    findWordFrequency(d->tree, wfs, n);
    return n;
}

// print a dictionary
void showDict(Dict d)
{
    // Inorder Traversal
    printTree(d->tree);
}
// Inorder Traversal 
void printTree(Link t)
{
    if (t == NULL) return; 
    printTree(t->left);
    printf("%d %s", t->data.freq, t->data.word);
    printTree(t->right);
}
// Finds the word frequence within a dict
// returns WFreq 
void findWordFrequency(Link t, WFreq *wfs, int n)
{
    if (t == NULL) return;
   
   findWordFrequency(t->left, wfs, n);

   int j = 0;
   while (t->data.freq <= wfs[j].freq && j < n) j++;
   for (int i = n - 1; i > j; i--) 
      wfs[i] = wfs[i-1];
   wfs[j] = t->data;

   findWordFrequency(t->right, wfs, n);
}
// Gets the height of the tree made from the dictionary 
int height(Link t) 
{
    if (t == NULL) return - 1;
    else {
        return t->height;
    }
}
// Right rotation around tree
Link rotateRight(Link t)
{
    // Node is NULL
    if (t == NULL) return t;
    // Base Case of Rotating Right
    Link leftChild = t->left;
    // If leftChild is NULL return the tree
    if (leftChild == NULL) return t;
    Link T2 = leftChild->right;
    leftChild->right = t;
    t->left = T2;
    // Updating the tree height for each node
    t->height = max(height(t->left), height(t->right)) + 1;
    leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;
    return leftChild;
}
// Left rotation around tree
Link rotateLeft(Link t)
{
    // Node is NULL
    if (t == NULL) return t;
    // Base Case of Rotating Right
    Link rightChild = t->right;
    // If rightChild is NULL return the tree
    if (rightChild == NULL) return t;
    Link T2 = rightChild->left;
    rightChild->left = t;
    t->right = T2;
    // Updating the tree height for each node
    t->height = max(height(t->left), height(t->right)) + 1;
    rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;
    return rightChild;
}
// Checks if the tree needs to be balanced
int Balance(Link t) 
{
    if (t == NULL) return 0;
    return height(t->left) - height(t->right);
}
// Returns the max value between a and b
int max(int a, int b) 
{
    return a > b ? a : b;
}

// Insert to the tree 
Link insert(Link t, char *w)
{
    if(t == NULL) return t = newNode(w);
	int cmp = strcmp(t->data.word, w);
	int balance = Balance(t);
	if(cmp > 0) {
		t->left = insertAVL(t->left, w);
		//  Case: Left Left
    	if (balance > 0) {
			// Left Right
			if (cmp < 0) t->left = rotateLeft(t->left);
			t = rotateRight(t);
		} 
	}
	if(cmp < 0) {
		t->right = insertAVL(t->right, w);
		// Right Right
    	if (balance < 0) {
			// Right Left
			if (cmp > 0) t->right = rotateRight(t->right);
			t = rotateLeft(t);
		}
	}
    // Correct the height of the sub tree
	t->height = max(height(t->left), height(t->right)) + 1;
	return t;
}

// Search for a particular point in the tree
Link search(Link t, char *w)
{
    Link find = NULL;
    if (t == NULL) return NULL;
    int cmp = strcmp(w, t->data.word);
    if (cmp == 0) return t;
    if(cmp < 0) find = search(t->right, w);
    else if(cmp > 0) find = search(t->left, w);
    return find;
}

