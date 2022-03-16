#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "file_helper.h"

int main()
{
    char* fileContent = 0;
    size_t length = 0;

    FILE* f = fopen("./wordTable.txt", "rb");
    assert(f);
    
    int res = readFile(f, &fileContent, &length);
    fclose(f);
    assert(res == 0);

    spair_t *pairs = (spair_t *)malloc(sizeof(spair_t));
    assert(pairs != NULL);

    size_t paircnt = 0;

    res = splitData(fileContent, length, ",", "\n", &pairs, &paircnt);
    assert(res == 0);

    free(pairs);
    return 0;
}