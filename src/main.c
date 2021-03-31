#include <stdio.h>
#include "numseq.h"

int main() {
    printf("Homework 2\n");
    if (numseq() == 1) {
        printf("Consistent realisation");
    } else if (numseq() == 2) {
        printf("Parallel realisation");
    }
    return 0;
}
