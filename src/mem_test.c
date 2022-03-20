#include <stdio.h>
#include <assert.h>
#include "my_malloc.h"

int main(int argc, char *argv[])
{

    printf("[ALLOCATE] requesting for %d bytes data.\n", 8160);
    char *a = my_malloc(8160);
    assert(a != NULL);
    printf("-----------------------------\n");

    printf("[ALLOCATE] requesting for %d bytes data.\n\n", 8000);
    char *b = my_malloc(8000);
    assert(b != NULL);
    printf("-----------------------------\n\n");

    printf("[DEALLOCATE] requesting for %d bytes data.\n", 4064);
    char *c = my_malloc(4064);
    assert(c != NULL);
    printf("-----------------------------\n\n");

    printf("[DEALLOCATE] requesting to free a variable\n");
    my_free(a);
    printf("-----------------------------\n\n");

    printf("[ALLOCATE] requesting to free b\n");
    my_free(b);
    printf("-----------------------------\n\n");

    printf("[ALLOCATE] requesting for %d bytes data.\n", 12300);
    char *d = my_malloc(12300);
    assert(d != NULL);
    printf("-----------------------------\n\n");

    printf("[DEALLOCATE] requesting to free c\n");
    my_free(c);
    printf("-----------------------------\n\n");
    printf("[DEALLOCATE] requesting to free d\n");
    my_free(d);
    printf("-----------------------------\n\n");

    return 0;
}
