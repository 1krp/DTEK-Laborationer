#include "randf.h"


int rand_num(int from, int to) {
    int diff = to - from ;

    int *p;
    p = (int*)malloc(sizeof(int)); 
    int res = ((int)p%diff)+from; 

    free(p);
    return res;
}
