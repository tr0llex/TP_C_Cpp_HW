#include <stdio.h>
#include "numseq.h"
#include <dlfcn.h>

int main() {
    printf("Simple function call: ");
    if (numseq() == 1) {
        printf("Consistent realisation\n");
    }

    void *library = 0;
    int (*numseq_p)();

    library = dlopen("./lib_num_sequence_parallel.so", RTLD_NOW);
    if (!library) {
        printf("%s", "Библиотека не найдена");
        return CODE_ERROR;
    }
    *(void **) (&numseq_p) = dlsym(library, "numseq");
    int a = (*numseq_p)();

    printf("dlsym function call: ");
    if (a == 2) {
        printf("Parallel realisation\n");
    }
    dlclose(library);
    return 0;
}
