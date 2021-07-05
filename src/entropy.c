#include "encoding.h"
#include "entropy.h"
#include <math.h>

float calc_entropy(node* head)
{
    double entropy=0;
    double total=0;
    node* temp=head;
    while(temp)
    {
        total+=temp->freq;
        temp=temp->next;
    }
    while(head)
    {
        entropy+=(head->freq/total)*(log2f(head->freq/total));
        head=head->next;
    }
    return -entropy;
}