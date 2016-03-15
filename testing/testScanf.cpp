#include <stdio.h>
#include <string.h>

int main() {

    char name[256] = {};

    scanf ("%[^\n]%*c", name);
    printf("[%s]\n", name);

    return 0;
}
