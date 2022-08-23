#ifndef HW01_TESTS_BLACK_MAGIC
#define HW01_TESTS_BLACK_MAGIC

/*****************************************************************************/
/*    YOU DO NOT HAVE TO READ NOR UNDERSTAND THIS FILE                       */
/*****************************************************************************/

/*
 * This file contains sorcery so dark even we are afraid of it.
 * If you try to read and understand this file, Satan himself will
 * spawn and wreak havoc in your room while playing Justin Bieber
 * and Whitney Houston simultaneously in an endless loop.
 *
 * You ONLY need to edit
 *  - hw01.c
 *  - hw01_tests.c (if you want custom tests)
 */

/*****************************************************************************/
/*    PROCEED AT YOUR OWN RISK                                               */
/*****************************************************************************/

static const char *current_name;

#define TEST(ID) \
    bool test_ ## ID(void)

#define TEST_FUNCTION(NAME) \
    bool(*FPTR)(char *, size_t) = & NAME

#define TEST_DONE \
    return true

#define SHOULD_PASS(STR)                \
    do {                                \
        if (!FPTR(STR, strlen(STR))) {  \
            printf("-> \"%s\"\n", STR); \
            return false;               \
        }                               \
    } while(0)

#define SHOULD_FAIL(STR)                \
    do {                                \
        if (FPTR(STR, strlen(STR))) {   \
            printf("-> \"%s\"\n", STR); \
            return false;               \
        }                               \
    } while(0)

#define TEST_RUN(ID)                    \
    do {                                \
        if (test_ ## ID())             \
            printf("test passed " #ID "\n"); \
        else                            \
            printf("test FAILED " #ID "\n"); \
    } while (0)

// Are you still alive? Good.

#endif // HW01_TESTS_BLACK_MAGIC
