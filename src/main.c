#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "file_helper.h"
#include "libclipboard.h"

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


    clipboard_c *cb = clipboard_new(NULL);
    char *last_cb_content = NULL;

    while(1)
    {
        #ifndef _WIN32
        #  include <unistd.h>
            sleep(1);
        #else
        #  include <windows.h>
            Sleep(1000);
        #endif

        char *cb_content = clipboard_text(cb);
        if (cb_content == NULL || cb_content == last_cb_content) continue;

        size_t i; bool reverse = false;
        for (i = 0; i < paircnt; i++)
        {
            if (strstr(pairs[i].first, cb_content) != NULL) break;
        }

        if (i == paircnt)
        {
            for (i = 0; i < paircnt; i++)
            {
                if (strstr(pairs[i].second, cb_content) != NULL) break;
            }
            reverse = true;
        }

        if (i == paircnt)
            clipboard_set_text(cb, "none");
        else if (reverse)
            clipboard_set_text(cb, pairs[i].first);
        else
            clipboard_set_text(cb, pairs[i].second);

        last_cb_content = realloc(last_cb_content, strlen(cb_content));
        strcpy(last_cb_content, cb_content);
    }


    free(pairs);
    return 0;
}