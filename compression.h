#ifndef compression_h_INCLUDED
#define compression_h_INCLUDED

#define MAX 16

// Code table regarding every character in the file
typedef struct codeTable
{
    char x;
    char code[MAX];
} codeTable;

// padding is done to ensure that the code generated for each charater will fit byte size.
// i.e : 4 byte + 3bits will be consider as 5 bits.
char padding;
unsigned char N;

#endif //compression_h_INCLUDED



