#include "file_helper.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>


/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
     (*dataptr) points to a dynamically allocated buffer, with
     (*sizeptr) chars read from the file.
     The buffer is allocated for one extra char, which is NULL,
     and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
*/
int readFile(FILE *in, char **dataptr, size_t *sizeptr)
{
    char  *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (in == NULL || dataptr == NULL || sizeptr == NULL)
        return READALL_INVALID;

    /* A read error already occurred? */
    if (ferror(in))
        return READALL_ERROR;

    while (1) {

        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            /* Overflow check. Some ANSI C compilers
               may optimize this away, though. */
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }

            temp = realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }

        n = fread(data + used, 1, READALL_CHUNK, in);
        if (n == 0)
            break;

        used += n;
    }

    if (ferror(in)) {
        free(data);
        return READALL_ERROR;
    }

    temp = realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';

    *dataptr = data;
    *sizeptr = used;

    return READALL_OK;
}


/* this functions splits a string into pairs of two, based on two seperator-strings.
    returns  0 if operation was successful
    returns -1 if one of the input parameters was NULL
    returns -2 if memory allocation failed
*/
int splitData(char *data, size_t size, const char *first_sep, const char *second_sep, spair_t **pairsptr, size_t *paircntptr)
{
    if (data == NULL || size == 0 || pairsptr == NULL || paircntptr == NULL) return -1;

    spair_t *pairs = (spair_t *)malloc(sizeof(spair_t));
    if (pairs == NULL) return -2;

    size_t paircnt = 0;

    pairs[paircnt].first = strtok(data, first_sep);

    while (pairs[paircnt].first != NULL)
    {
        pairs[paircnt++].second = strtok(NULL, second_sep);
        pairs = realloc(pairs, (paircnt + 1) * sizeof(spair_t));
        pairs[paircnt].first = strtok(NULL, first_sep);
    }

    *pairsptr = pairs;
    *paircntptr = paircnt;

    return 0;
}