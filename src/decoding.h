#ifndef decoding_h_INCLUDED
#define decoding_h_INCLUDED

char *decodeBuffer(char buffer);
char *int2string(int n);
int match(char a[],char b[],int limit);
int fileError(FILE *fp);

#endif //decoding_h_INCLUDED
