#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "file_helper.h"
#include "libclipboard.h"

#ifndef _WIN32
#  include <unistd.h>
#else
#  include <windows.h>
#endif

int main()
{
    char* fileContent = 0;
    size_t length = 0;

    // open file
    FILE* f = fopen("./wordTable.txt", "rb");
    assert(f);
    
    // copy file content into string and file-length into variable
    int res = readFile(f, &fileContent, &length);
    // close file now that content is in string
    fclose(f);
    assert(res == 0);

    // create dynamic string-pair array
    spair_t *pairs = (spair_t *)malloc(sizeof(spair_t));
    assert(pairs != NULL);

    // pair-count
    size_t paircnt = 0;

    // split the file-content-string into pairs 
    res = splitData(fileContent, length, ",", "\n", &pairs, &paircnt);
    assert(res == 0);

    // create clipboard object
    clipboard_c *cb = clipboard_new(NULL);
    
    char *cb_content = clipboard_text(cb);
    // clipboard content of last iteration
    char *last_cb_content = cb_content;

    while(1)
    {
        // different sleep functions dependent on os
        #ifndef _WIN32
            sleep(1);
        #else
            Sleep(1000);
        #endif
    }

    // free clipboard and other pointers
    clipboard_free(cb);
    free(last_cb_content);
    free(pairs);
    return 0;
}