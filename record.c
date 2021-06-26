#include "record.h"
#include <stdio.h>

void record(long int in_size,float entropy,long int out_size,double time)
{
    FILE *res=fopen("res.csv","w+");
    fprintf(res,"%ld, %f, %ld, %lf\n",in_size,entropy,out_size,time);
    fclose(res);
}