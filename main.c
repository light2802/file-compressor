/*
File Name: main_c
Author: Aarya Chaumal
*/

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "read_directory.h"

//Problem: Main File for Project zip

int main(int argc, char** argv)
{
    DIR *folder;
    char* loaction;
    int compression_type=0;
    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-a"))
        {
            strcpy(loaction,".");
        }
    }
    folder = opendir(".");
    return 0;
}
