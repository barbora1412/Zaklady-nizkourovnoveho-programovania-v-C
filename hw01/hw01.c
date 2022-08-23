#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "hw01.h"

#define STACK_SIZE 10
#define STACK_EMPTY (-1)

// http://www.cprograms.in/Stack/push-pop-stack.html
/*PUSH FUNCTION*/
bool push( char stack[],int *top, char *data)
{
    if(*top == STACK_SIZE -1 ) {
        return false;
    }
    *top = *top + 1;
    stack[*top] = *data;
    return true;
}

bool pop( char stack[], int *top, char *data)
{
    if( *top == STACK_EMPTY ) {
        return false;
    }
    *data = stack[*top];
    stack[*top] = '\0';
    *top = *top - 1;
    return true;
}

/****  UTILITIES  ************************************************************/

/*
 * Returns true if and only if 'c' is one of the characters of 'str'.
 *
 * DO NOT EDIT THIS FUNCTION, it is not part of your homework.
 * You can, however, use it to make your life easier, like this:
 *
 *      if (isOneOf(c, str)) {
 *          // this will execute if c is contained in str
 *      } else {
 *          // this will execute otherwise
 *      }
 *
 * For instance,
 *      isOneOf(' ', "Hello, World!")   -> true
 *      isOneOf(' ', "Pocahontas")      -> false
 *      isOneOf('.', "Hello, World!")   -> false
 *      isOneOf('!', "Hello, World!")   -> true
 */
bool isOneOf(char c, char str[])
{
    return strchr(str, c) != NULL;
}

/****  HERE BEGINS YOUR HOMEWORK  ********************************************/

/*
 * There shall be no two consecutive spaces in the string.
 * You are to FIND and FIX an error in this code.
 *
 * Examples:
 *  Wrong: "Hello  world."
 *  Wrong: "Hello.  World."
 *  Right: "Hello world."
 *  Right: "Hello. Wonderful. World."
 */
bool noTwoConsecutiveSpaces(char str[], size_t len)
{
    if (len < 2) {
        return true;
    }
    for (size_t i = 1; i < len; i++) {
        if (isspace(str[i]) && isspace(str[i - 1]))
            return false;
    }
    return true;
}

/**
 * The string represents a list of sentences. A sentence ends with with a
 * '.', '?', or a '!' sign, and starts with the first nonspace character
 * in the string or after the end of the previous sentence.
 *
 * Hint: see 'isupper' function
 */
bool sentenceStartsWithUppercase(char str[], size_t len)
{
    int  sentence = 0;
    for (size_t i = 0; i < len; i++) {
        if (isspace(str[i]) || isOneOf(str[i], "(){}[]")) {
            continue;
        }
        if (isupper(str[i]) && sentence == 0) {
            sentence = 1;
            continue;
        }
        if (isOneOf(str[i], ".?!") && sentence == 1) {
            sentence = 0;
            continue;
        }
        if (sentence == 1 ) {
            continue;
        }
        return false;
    }
    return true;
}

/*
 * Every sentence in the string ends with '.', '?' or a '!'.
 */
bool allSentencesAreClosed(char str[], size_t len)
{
    int  sentence = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isOneOf(str[i], " (){}[]") && sentence == 0) {
            sentence = 1;
        }
        if (isOneOf(str[i], ".?!") && sentence == 1) {
            sentence = 0;
        }
    }
    return sentence == 0;
}

/**
 * There is no separator with a space before it.
 *
 *  Wrong: "Hello, World !"
 *  Wrong: "Hello , World!"
 *  Right: "Hello, World!"
 */
bool noSpaceBeforeSeparators(char str[], size_t len)
{
    if (len < 2) {
        return true;
    }
    for (size_t i = 1; i < len; i++) {
        if (isOneOf(str[i], ".,?!") && isspace(str[i - 1]))
            return false;
    }
    return true;
}

/**
 * Every seprator (i.e. '.', ',', '!' or '?') is followed by a space,
 * with the exception of the separator is the last character in the string.
 *
 *  Wrong: "Hello,world."
 *  Right: "Hello, world."
 */
bool spaceAfterSeparators(char str[], size_t len)
{
    for (size_t i = 1; i < len; i++) {
        if (isOneOf(str[i - 1], ".,?!") && !isspace(str[i]))
            return false;
    }
    return true;
}

/**
 * Every opening bracket '(' is properly closed with ')' and there are no
 * nested parentheses.
 *
 *  Wrong: ":("
 *  Right: "Hello world (again)!"
 *  Wrong: "Hello, (tiny (puny)) world!"
 */
bool properParenthesesPairing(char str[], size_t len)
{
    /**
     *  brackets () is 1
     *  brackets [] is 2
     *  brackets {} is 3
     */
    int bracket = 0;
    if (len == 1 && isOneOf(str[0], "({[")) {
        return false;
    }
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '(' && bracket == 0) {
            bracket = 1;
            continue;
        }
        if (str[i] == '[' && bracket == 0) {
            bracket = 2;
            continue;
        }
        if (str[i] == '{' && bracket == 0) {
            bracket = 3;
            continue;
        }
        if (str[i] == ')' && bracket == 1) {
            bracket = 0;
            continue;
        }
        if (str[i] == ']' && bracket == 2) {
            bracket = 0;
            continue;
        }
        if (str[i] == '}' && bracket == 3) {
            bracket = 0;
            continue;
        }
        if (isOneOf(str[i], "(){}[]")) {
            return false;
        }
    }
    return bracket == 0;
}

/****  BONUS  ****************************************************************/

/*
 * You are not required to implement the following functions, but you
 * can get up to 3 bonus points if you do it correctly.
 */

/**
 * Parentheses should not cross sencentes. That is, if a bracket is opened
 * outside of a sentence, it must be closed outside as well.
 * On the other hand, a bracket opened inside a sentence has to be
 * closed before the sentence ends.
 *
 *  Right: "(First sentence. Second sentence.) Third sentence."
 *  Right: "First (inside) sentence."
 *  Wrong: "First (sentence. Second) sentence."
 *  Wrong: "(First sentence. Second) sentence."
 */
bool noParenthesesSentenceCrossing(char str[], size_t len)
{
    int sentence = 0;
    char stack[STACK_SIZE] = {0}; // zo stackoverflow - naplni zasobnik nulami aby am neboli nahodne veci
    int top = STACK_EMPTY;
    char achar;

//    if (!properParenthesesPairing(str,len)) {
//        return false;
//    }
    for (size_t i = 0; i < len; i++) {
        if (isOneOf(str[i], "({[")) {
            achar = 'z'; //v zasobniku je zatvorka
            if (top == STACK_EMPTY || stack[top] != achar) {
                push(stack, &top, &achar);
//                continue;
            }
        }
        if (isOneOf(str[i], ")}]")) {
            achar = 'z';
            if (stack[top] == achar) {
                pop(stack, &top, &achar);
//                continue;
            }
            else {
                return false;
            }
        }
        if (!isOneOf(str[i], " (){}[]") && sentence == 0) {
            achar = 'v'; //v zasobniku je veta
            if (top == STACK_EMPTY || stack[top] != achar) {
                push(stack, &top, &achar);
                sentence = 1;
            }
         }
        if (isOneOf(str[i], ".?!")) {
            achar = 'v';
            if (stack[top] == achar) {
                pop(stack, &top, &achar);
                sentence = 0;
//                continue;
            }
            else {
                return false;
            }
        }
    }

    return top == STACK_EMPTY;
}

/**
 * There can be double quotes inside the text '"', but always in pair.
 * Opening quote must not be followed by a space and similarly
 * there can be no space before the closing quote.
 */
bool noSpaceBeforeClosingOrAfterOpeningApostrophe(char str[], size_t len)
{
    int apostroph = 0;
    if (len == 0) {
        return true;
    }
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '"') {
            if (apostroph == 0) {
                if (isspace(str[i + 1])) {
                    return false;
                }
                if (!isspace(str[i + 1])) {
                    apostroph = 1;
                    continue;
                }
            }
            if (apostroph == 1) {
                if (isspace(str[i - 1])) {
                    return false;
                }
                if (!isspace(str[i - 1])) {
                    apostroph = 0;
                    continue;
                }
            }
        }
    }
    return apostroph == 0;
}
