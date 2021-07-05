#ifndef compression_h_INCLUDED
#define compression_h_INCLUDED

#define MAX 16

// Code table regarding every character in the file
typedef struct codeTable
{
    char x;
    char code[MAX];
} codeTable;


void restore_extension(char* str);

#endif //compression_h_INCLUDED



