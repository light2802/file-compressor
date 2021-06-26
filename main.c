/*
File Name: main_c
Author: Aarya Chaumal
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "encoding.h"
#include "decoding.h"
#include "compression.h"
#include "entropy.h"
#include "size.h"
#include "record.h"

extern char padding;
extern unsigned char N;

//Head and root nodes  in the linked list.
node* HEAD = NULL;
node* ROOT = NULL;

char compressed_extension[]  = ".aar";

codeTable *codelist;
int n;
//Global variables

//Problem: Zip files using huffman coding

int main(int argc, char** argv)
{
    clock_t start,end;
    if(argc<=3)
    {
        int choice=0;
        choice=strcmp(argv[1],"-e")==0?1:0;              //1->encode (compress)
        choice=strcmp(argv[1],"-d")==0?2:choice;        //2->decode (decompress)
        switch(choice)
        {
            case 1:
            {
                FILE *fp, *fp1;
                char ch;
                int t;
                printf("\n***Automated File Compression***\n");
                printf("\nCreating new compressed file...............\n");
                argv[3]=(char *)malloc(sizeof(char)*(strlen(argv[2])+(strlen(compressed_extension))+1));
                strcpy(argv[3],argv[2]);
                strcat(argv[3],compressed_extension);
                argc++;
                fp = fopen(argv[2],"rb");
                if(fp == NULL)
                {
                    printf("Error, Input file does not exists, Check the file name\n");
                    return -1;
                }

                start=clock();

                printf("Initiating the compression sequence.................\n");
                printf("Reading input file %s\n",argv[2]);
                while(fread(&ch,sizeof(char),1,fp)!=0)
                    addSymbol(ch);
                fclose(fp);
                printf("Constructing Huffman-Tree....................\n");
                makeTree();
                printf("Assigning codewords..........................\n");
                //Pre order traversal of the of the huffman code.
                genCode(ROOT,"\0");
                printf("Compressing the file.........................\n");
                fp=fopen(argv[2],"r");
                if(fp==NULL)
                {
                    printf("\n[!]Input file cannot be opened.\n");
                    return -1;
                }
                fp1=fopen(argv[3],"wb");
                if(fp1==NULL)
                {
                    printf("\n[!]Output file cannot be opened.\n");
                    return -2;
                }
                printf("\nReading input file %s.......................",argv[2]);
                printf("\nWriting file %s........................",argv[3]);
                printf("\nWriting File Header..............................");
                    writeHeader(fp1);
                printf("\nWriting compressed content............................");
                //writing corersponding codes into the new file fp1
                while(fread(&ch,sizeof(char),1,fp)!=0)
                    writeCode(ch,fp1);
                fclose(fp);
                fclose(fp1);
                printf("\n***Done***\n");

                end=clock();
                double time = (double)(end - start) / (double)(CLOCKS_PER_SEC);
                float entropy = calc_entropy(HEAD);
                FILE* input=fopen(argv[2],"r");
                FILE* output=fopen(argv[3],"r");
                long double in_size=calc_file_size(input),out_size=calc_file_size(output);
                fclose(input);
                fclose(output);
                printf("Entropy : %f\n",entropy);
                printf("Time to make tree and generate output : %lf sec\n",time);
                printf("Input file size : %ld bytes\n",in_size);
                printf("Output file size : %ld bytes\n",out_size);

                record(in_size,entropy,out_size,time);
                return 0;
            }
            case 2:
            {
                FILE *fp,*outfile;
                char buffer;
                char *decoded;
                int i;
                printf("***Huffman Decoding***\n");
                argv[3]=(char *)malloc(sizeof(char)*(strlen(argv[1])+4+1));
                strcpy(argv[3],argv[2]);
                restore_extension(argv[3]);
                printf("\n!!!!%s\n",argv[3]);
                argc++;
                if((fp=fopen(argv[2],"rb"))==NULL)
                {
                    printf("[!]Input file cannot be opened.\n");
                    return -1;
                }

                printf("\n[Reading File Header]");
                if(fread(&buffer,sizeof(unsigned char),1,fp)==0) return (fileError(fp));
                    N=buffer;		//No. of structures(mapping table records) to read
                if(N==0)
                    n=256;
                else
                    n=N;
                printf("\nDetected: %u different characters.",n);

                //allocate memory for mapping table
                codelist=(codeTable *)malloc(sizeof(codeTable)*n);

                printf("\nReading character to Codeword Mapping Table");
                if(fread(codelist,sizeof(codeTable),n,fp)==0)
                    return (fileError(fp));
                    //Read mapping table


                if(fread(&buffer,sizeof(char),1,fp)==0) return (fileError(fp));
                    padding=buffer;		//No. of bits to discard
                printf("\nDetected: %u bit padding.",padding);

                if((outfile=fopen(argv[3],"wb"))==NULL)
                {
                    printf("[!]Output file cannot be opened.\n");
                    fclose(fp);
                    return -2;
                }

                printf("\n\n[Reading data]\nReplacing codewords with actual characters");
                while(fread(&buffer,sizeof(char),1,fp)!=0)	//Read 1 byte at a time
                {
                    decoded=decodeBuffer(buffer);	//decoded is pointer to array of characters read from buffer byte
                    i=0;
                    while(decoded[i++]!='\0');	//i-1 characters read into decoded array
                    fwrite(decoded,sizeof(char),i-1,outfile);
                }
                fclose(fp);
                fclose(outfile);
                printf("\nEverything fine..\nOutput file %s written successfully.\n",argv[2]); 
                return 0;
            }
        }

    }
    else
        printf("Invalid parameters!!!");
    return 0;
}
