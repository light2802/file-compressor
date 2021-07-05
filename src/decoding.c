#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "decoding.h"
#include "compression.h"

extern codeTable *codelist;
extern int n;
extern char padding;
extern unsigned char N;

char *decodeBuffer(char b)
{
    int i=0,j=0,t;
    static int k;
    static int buffer;	// buffer larger enough to hold two b's
    char *decoded=(char *)malloc(MAX*sizeof(char));
    /*
    Logic:
    buffer = [1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0]
            k
    b   =        [ 1 0 1 1 0 0 1 1 ]
    //put b in integer t right shift k+1 times then '&&' with buffer; k=k+8;
    buffer = [1 0 0 1 1 0 1 1 0 0 1 1 0 0 0 0]
                    k
    */

    t=(int)b;
    
    t=t & 0x00FF;		//mask high byte

    t=t<<8-k;		//shift bits keeping zeroes for old buffer 

    buffer=buffer | t;	//joined b to buffer
    k=k+8;			//first useless bit index +8 , new byte added

    if(padding!=0)	// on first call
    {
        buffer=buffer<<padding;
        k=8-padding;	//k points to first useless bit index
        padding=0;
    }

    //loop to find matching codewords
    while(i<n)
    {
        if(!match(codelist[i].code, int2string(buffer),k))
        {	
            decoded[j++]=codelist[i].x;	//match found inserted decoded
            t=strlen(codelist[i].code);	//matched bits
            buffer=buffer<<t;		//throw out matched bits
            k=k-t;				//k will be less
            i=0;				//match from initial record
            if(k==0) break;
            continue;
        }
        i++;
    }

    decoded[j]='\0';
    return decoded;

}//Finallly :)))))

int match(char a[],char b[],int limit)
{
    b[strlen(a)]='\0';
    b[limit]='\0';
    return strcmp(a,b);
}

char *int2string(int n)
{
    int i,k,andd,j;
    char *temp=(char *)malloc(16*sizeof(char));
    j=0;

    for(i=15;i>=0;i--)
    {
        andd=1<<i;
        k=n & andd;
        if(k==0) temp[j++]='0'; else temp[j++]='1';
    }
    temp[j]='\0';
    return temp;
}

int fileError(FILE *fp)
{
    printf("[!]File read Error.\n[ ]File is not compressed using aar command.\n");
    fclose(fp);
    return -3;
}
