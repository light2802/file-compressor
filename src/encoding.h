#ifndef encoding_h_INCLUDED
#define encoding_h_INCLUDED

//Internal and Leaf nodes
#define LEAF 0
#define INTERNAL 1
#include <stdio.h>
typedef struct node         //Its a singly linked list node as well as a binary tree nodes
{                          //This struct helps to convert the singly linked list directly to the tree
    char x;
    unsigned int freq;
    char* code;
    int type;
    struct node* next;
    struct node* left;
    struct node* right;

} node;

void printll();
void makeTree();
void genCode(node *p,char* code);
void insert(node *p,node *m);
void addSymbol(char c);
void writeHeader(FILE *f);
void writeBit(int b,FILE *f);
void writeCode(char ch,FILE *f);
char *getCode(char ch);

#endif //encoding_h_INCLUDED
