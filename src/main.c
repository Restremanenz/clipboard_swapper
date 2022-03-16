#include <stdio.h>
#include "file_helper.h"

int main()
{
    char* fileContent = 0;
    size_t length = 0;

    FILE* f = fopen("./wordTable.txt", "rb");
    
    int res = readFile(f, &fileContent, &length);

    printf("%s\n", fileContent);

    return 0;
}