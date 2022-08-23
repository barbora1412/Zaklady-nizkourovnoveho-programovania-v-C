#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hw01.h"

/*****************************************************************************/
/*    YOU DO NOT HAVE TO READ NOR UNDERSTAND THIS FILE                       */
/*****************************************************************************/

// You ONLY need to edit
//   - hw01.c
//   - hw01_tests.c (if you want custom tests)

// But hey, if you are curious, then by all means, go ahead and have a look.
// There are some comments that might help you understand what is going on.

/*****************************************************************************/

/**
 * \file    hw01_main.c
 * \author  Jan Tusil <xtusil@fi.muni.cz>
 */

/**
 * Number of bytes used by \c file_slurp to grow chunk being read from
 * the file.
 *
 * The growth process is advised to be exponential (e.g. 8,16,32,...),
 * but this homework is so simple it would be an overkill to implement.
 */
const size_t SLURP_GROW_SIZE = 64U;

/**
 * Reads the entire content of the given \c file.
 *
 * \param   file        file handle to read from
 * \returns pointer to allocated memory chunk or \c NULL in case of error
 *
 * \note    the caller is responsible for releasing the chunk
 */
char *file_slurp(FILE *file)
{
    char   *chunk    = NULL; // current input chunk pointer
    size_t  capacity = 0U;   // current chunk capacity
    size_t  index    = 0U;   // number of used bytes
    size_t  rd_bytes = 0U;   // partial bytes count from fread

    do {
        // grow the allocated pool
        index = capacity;
        capacity += SLURP_GROW_SIZE;
        char *nptr = realloc(chunk, capacity);

        // allocation may fail
        if (!nptr) {
            free(chunk);
            return NULL;
        }

        chunk = nptr;

        // fill the newly allocated chunk of memory
        rd_bytes = fread(chunk + index, sizeof(char), SLURP_GROW_SIZE, file);

        // fread returns less than required number of bytes
        // when there is nothing else to read or an error occurs
    } while (rd_bytes == SLURP_GROW_SIZE);

    // we could have got out of the loop because of an error
    // then the result may be corrupt, better get rid of it
    if (ferror(file)) {
        free(chunk);
        return NULL;
    }

    // do not forget to add the terminating zero before returning
    chunk[index + rd_bytes] = '\0';
    return chunk;
}

/**
 * This is the main program entry. It reads everything from the
 * standard input and runs the spell checks.
 */
int main(void)
{
    // read the entire standard input
    char *text = file_slurp(stdin);

    if (!text) {
        perror("input error");
        return 1;
    }

    size_t len = strlen(text);

    // run all spell checks one by one
    #define CHECK_WITH(f)          \
    if (f(text, len)) {            \
        printf("correct   %s\n", #f); \
    } else {                       \
        printf("INCORRECT %s\n", #f); \
    }

    CHECK_WITH(sentenceStartsWithUppercase);
    CHECK_WITH(noSpaceBeforeSeparators);
    CHECK_WITH(noTwoConsecutiveSpaces);
    CHECK_WITH(spaceAfterSeparators);
    CHECK_WITH(properParenthesesPairing);
    CHECK_WITH(noParenthesesSentenceCrossing);
    CHECK_WITH(allSentencesAreClosed);
    CHECK_WITH(noSpaceBeforeClosingOrAfterOpeningApostrophe);

    // we are done
    free(text);
    return 0;
}
