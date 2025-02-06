#include <stdio.h>
#include <stdlib.h>

int main() {
    char *p = malloc(10);
    printf("Hello, world!\n");
    free(p);
    return 0;
}
