#ifndef encoding_h_INCLUDED
#define encoding_h_INCLUDED

//Internal and Leaf nodes
#define LEAF 0
#define INTERNAL 1

typedef struct node 
{
    char x;
    int freq;
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
