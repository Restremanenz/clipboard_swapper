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
    // clipboard content of last iteration
    char *last_cb_content = NULL;

    while(1)
    {
        // different sleep functions dependent on os
        #ifndef _WIN32
            sleep(1);
        #else
            Sleep(1000);
        #endif

        // read clipboard content into string
        char *cb_content = clipboard_text(cb);
        // check if clipboard content is NULL or changed since last iteration
        if (cb_content == NULL || cb_content == last_cb_content) continue;

        // check if clipboard content matches a word of the first string of the pairs
        size_t i; bool reverse = false;
        for (i = 0; i < paircnt; i++)
        {
            if (strstr(pairs[i].first, cb_content) != NULL) break;
        }

        // didn't found any matching words
        if (i == paircnt)
        {
            // check if clipboard content matches a word of the second string of the pairs
            for (i = 0; i < paircnt; i++)
            {
                if (strstr(pairs[i].second, cb_content) != NULL) break;
            }
            reverse = true;
        }
        // didn't found any matching words
        if (i == paircnt)
            clipboard_set_text(cb, "none");
        // found matching word in the second string of the pairs
        else if (reverse)
            clipboard_set_text(cb, pairs[i].first);
        // found matching word in the first string of the pairs
        else
            clipboard_set_text(cb, pairs[i].second);

        // update the last clipboard content variable
        last_cb_content = realloc(last_cb_content, strlen(cb_content));
        strcpy(last_cb_content, cb_content);
    }

    // free clipboard and other pointers
    clipboard_free(cb);
    free(last_cb_content);
    free(pairs);
    return 0;
}