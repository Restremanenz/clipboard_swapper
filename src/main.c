#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "file_helper.h"
#include "libclipboard.h"

#ifndef _WIN32
#  include <unistd.h>
#else
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0500
#endif
#  include <windows.h>
#endif

#define SEP_FIRST ","  // only one character!
#define SEP_SECOND "\n" // only one character!

int main()
{
    #ifdef _WIN32
        HWND hWnd = GetConsoleWindow();
        ShowWindow( hWnd, SW_MINIMIZE );  //won't hide the window without SW_MINIMIZE
        ShowWindow( hWnd, SW_HIDE );
    #endif

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
    res = splitData(fileContent, length, SEP_FIRST, SEP_SECOND, &pairs, &paircnt);
    assert(res == 0);
    
    #ifdef LIBCLIPBOARD_BUILD_X11
        // create clipboard object
        clipboard_c *cb = clipboard_new(NULL);
        assert(cb != NULL);
    #endif

    // clipboard content of last iteration
    char *last_cb_content = malloc(1);

    while(1)
    {   
        // different sleep functions dependent on os
        #ifndef _WIN32
            sleep(1);
        #else
            Sleep(1000);
        #endif

        #ifndef LIBCLIPBOARD_BUILD_X11
            // create clipboard object
            clipboard_c *cb = clipboard_new(NULL);
            assert(cb != NULL);
        #endif

        // length of the clipboard content
        int len = 0;
        // clipboard content
        char *cb_content = clipboard_text_ex(cb, &len, LCB_CLIPBOARD);

        // check if content is NULL or changed since last iteration
        if (cb_content == NULL || strcmp(last_cb_content, cb_content) == 0) continue;

        // index of matching word (if existing)
        size_t i;
        // if second word is matching clipboard content
        bool reverse = false;

        // check if first word of pairs is matching clipboard content
        for (i = 0; i < paircnt; i++)
            if (strstr(pairs[i].first, cb_content) != NULL) break;
        
        // didn't find any matching words in first word of pairs
        if (i == paircnt)
        {
            reverse = true;
            // check if second word of pairs is matching clipboard content
            for (i = 0; i < paircnt; i++)
                if (strstr(pairs[i].second, cb_content) != NULL) break;
        }

        // didn't find any matching word in second word of pairs either
        if (i == paircnt)
        {
            cb_content = realloc(cb_content, 5 * sizeof(char));
            assert(cb_content != NULL);
            strcpy(cb_content, "none");
        }
        // found matching word in second word of pairs
        else if (reverse)
        {
            cb_content = realloc(cb_content, (strlen(pairs[i].first) + 1) * sizeof(char));
            assert(cb_content != NULL);
            strcpy(cb_content, pairs[i].first);
        }
        // found matching word in first word of pairs
        else
        {
            cb_content = realloc(cb_content, (strlen(pairs[i].second) + 1) * sizeof(char));
            assert(cb_content != NULL);
            strcpy(cb_content, pairs[i].second);
        }

        // copy the matching word to clipboard
        res = clipboard_set_text_ex(cb, cb_content, strlen(cb_content), LCB_CLIPBOARD);
        assert(res == true);

        // update the last clipboard content with the current content
        last_cb_content = realloc(last_cb_content, len * sizeof(char) + 1);
        assert(last_cb_content != NULL);
        strcpy(last_cb_content, cb_content);

        // free the variable that holds current clipboard content
        free(cb_content);

        #ifndef LIBCLIPBOARD_BUILD_X11
        // free the clipboard for this iteration
        clipboard_free(cb);
        #endif
    }

    // free last clipboard content and word pairs
    free(last_cb_content);
    free(pairs);

    #ifdef LIBCLIPBOARD_BUILD_X11
        // free the clipboard for this iteration
        clipboard_free(cb);
    #endif
    return 0;
}