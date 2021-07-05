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
    char* res_name="results.csv";
    int input_index;
    int choice=1;
    if(argc<=5)
    {
        for(int i=1; i<argc; i++)
        {
            if(strcmp(argv[i],"-e")==0)
            {
                choice=1;
                input_index=i+1;
                i+=2;
                continue;
            }
            if(strcmp(argv[i],"-d")==0)
            {
                choice=2;
                input_index=i+1;
                i+=2;
                continue;
            }
            if(strcmp(argv[i],"-r")==0)
            {
                strcpy(res_name,argv[i+1]);
                i+=2;
                continue;
            }
        }
    }
    else
        printf("Invalid parameters!!!");
    switch(choice)
    {
        case 1:
        {
            FILE *fp, *fp1;
            char ch;
            int t;
            printf("\n***Automated File Compression***\n");
            printf("\nCreating new compressed file...............\n");
            argv[argc+1]=(char *)malloc(sizeof(char)*(strlen(argv[input_index])+(strlen(compressed_extension))+1));
            strcpy(argv[argc+1],argv[input_index]);
            strcat(argv[argc+1],compressed_extension);
            argc++;
            fp = fopen(argv[input_index],"rb");
            if(fp == NULL)
            {
                printf("Error, Input file does not exists, Check the file name\n");
                return -1;
            }

            start=clock();

            printf("Initiating the compression sequence.................\n");
            printf("Reading input file %s\n",argv[input_index]);
            while(fread(&ch,sizeof(char),1,fp)!=0)
                addSymbol(ch);
            fclose(fp);
            printf("Constructing Huffman-Tree....................\n");
            makeTree();
            printf("Assigning codewords..........................\n");
            //Pre order traversal of the of the huffman code.
            genCode(ROOT,"\0");
            printf("Compressing the file.........................\n");
            fp=fopen(argv[input_index],"r");
            if(fp==NULL)
            {
                printf("\n[!]Input file cannot be opened.\n");
                return -1;
            }
            fp1=fopen(argv[argc+1],"wb");
            if(fp1==NULL)
            {
                printf("\n[!]Output file cannot be opened.\n");
                return -2;
            }
            printf("\nReading input file %s.......................",argv[input_index]);
            printf("\nWriting file %s........................",argv[argc+1]);
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
            FILE* input=fopen(argv[input_index],"r");
            FILE* output=fopen(argv[argc+1],"r");
            long int in_size=calc_file_size(input),out_size=calc_file_size(output);
            fclose(input);
            fclose(output);
            printf("Entropy : %f\n",entropy);
            printf("Time to make tree and generate output : %lf sec\n",time);
            printf("Input file size : %ld bytes\n",in_size);
            printf("Output file size : %ld bytes\n",out_size);

            record(res_name,in_size,entropy,out_size,time);
            return 0;
        }
        case 2:
        {
            FILE *fp,*outfile;
            char buffer;
            char *decoded;
            int i;
            printf("***Huffman Decoding***\n");
            argv[argc+1]=(char *)malloc(sizeof(char)*(strlen(argv[1])+4+1));
            strcpy(argv[argc+1],argv[input_index]);
            restore_extension(argv[argc+1]);
            printf("\n!!!!%s\n",argv[argc+1]);
            argc++;
            if((fp=fopen(argv[input_index],"rb"))==NULL)
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

            if((outfile=fopen(argv[argc+1],"wb"))==NULL)
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
            printf("\nEverything fine..\nOutput file %s written successfully.\n",argv[input_index]); 
            return 0;
        }
    }
    return 0;
}
