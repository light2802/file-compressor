#include <stdio.h>
#include "size.h"

long int calc_file_size(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    return (long int)ftell(file);
}