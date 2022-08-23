#ifndef HW01_H
#define HW01_H

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t

/*****************************************************************************/
/*    YOU DO NOT HAVE TO READ NOR UNDERSTAND THIS FILE                       */
/*****************************************************************************/

// You ONLY need to edit
//   - hw01.c
//   - hw01_tests.c (if you want custom tests)

/*****************************************************************************/

bool noTwoConsecutiveSpaces(char str[], size_t len);
bool sentenceStartsWithUppercase(char str[], size_t len);
bool allSentencesAreClosed(char str[], size_t len);
bool noSpaceBeforeSeparators(char str[], size_t len);
bool spaceAfterSeparators(char str[], size_t len);
bool properParenthesesPairing(char str[], size_t len);
bool noParenthesesSentenceCrossing(char str[], size_t len);
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t len);

#endif // HW01_H
