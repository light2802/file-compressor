#include <string.h>

// padding is done to ensure that the code generated for each charater will fit byte size.
// i.e : 4 byte + 3bits will be consider as 5 bits.
char padding;
unsigned char N;

void restore_extension(char* str)
{
    int len = strlen(str);
    int i;
    for(i = len-1; i >=0; i--)
        if(str[i] == '.')
            break;
    str[i++] = '\0';
    while(i < len)
        str[i++] = '\0';
}
