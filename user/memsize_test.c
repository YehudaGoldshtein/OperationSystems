#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

    printf("memsize: %d\n", memsize());
    //use maloc to allocate memory of size 20k bytes
    char *p = malloc(20*1024);
    printf("memsize: %d\n", memsize());
    //free the allocated memory
    free(p);
    printf("memsize: %d\n", memsize());
    exit(0);
}