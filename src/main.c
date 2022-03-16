#include <stdio.h>
#include <assert.h>
#include "file_helper.h"

int main()
{
    char* fileContent = 0;
    size_t length = 0;

    FILE* f = fopen("./wordTable.txt", "rb");
    assert(f);
    
    int res = readFile(f, &fileContent, &length);
    assert(res == 0);

    printf("%s\n", fileContent);

    return 0;
}