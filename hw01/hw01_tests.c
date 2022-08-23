#include <stdio.h>  // printf
#include <string.h> // strlen

#include "hw01.h"
#include "hw01_tests.h"

/*
 * This file uses a minimalistic testing framework. You are not required to
 * understand it perfectly and you will see a much better one
 * with more features later in the course.
 *
 * For now, you can write simple tests below lines marked with TODO.
 * All you need are two functions(*) SHOULD_PASS and SHOULD_FAIL.
 *
 * For instance,
 *
 *      TEST_FUNCTION(noTwoConsecutiveSpaces);
 *
 * says that the function being tested is "noTwoConsecutiveSpaces".
 * Then,
 *
 *      SHOULD_PASS("Hello, World");
 *
 * requires that the function returns true on "Hello, World".
 * Otherwise the test fails and you need to fix your function.
 * Similartly,
 *
 *      SHOULD_FAIL("Hello,  world");
 *
 * asserts that noTwoConsecutiveSpaces returns false on given input,
 * otherwise the test fails. You can call as many SHOULD_PASS or SHOULD_FAIL
 * as you like, with different parameters.
 *
 * (*) Those are not really functions, but sssh!
 */

TEST(noTwoConsecutiveSpaces) {
    TEST_FUNCTION(noTwoConsecutiveSpaces);

    SHOULD_PASS("Ahoj , , ");
    SHOULD_PASS("");
    SHOULD_PASS(".");
    SHOULD_PASS("Hello, world");
    SHOULD_PASS(" Hello, world");
    SHOULD_PASS(" , ");

    SHOULD_FAIL("Hello,  world");
    SHOULD_FAIL("  Hello, world");
    SHOULD_FAIL("  ");
    SHOULD_FAIL("blabla  ");
    SHOULD_FAIL("   ");

    TEST_DONE;
}

TEST(sentenceStartsWithUppercase) {
    TEST_FUNCTION(sentenceStartsWithUppercase);

    SHOULD_PASS("Ahoj konicci.");
    SHOULD_PASS("  Ahoj,. Cau");
    SHOULD_PASS("");
    SHOULD_PASS("A");
    SHOULD_PASS("{");
    SHOULD_PASS(" ");
    SHOULD_PASS("{A");

    SHOULD_FAIL("Ahoj!    3cau");
    SHOULD_FAIL("\"Ahoj");
    SHOULD_FAIL(" , Ahoj");
    SHOULD_FAIL(".A");
    SHOULD_FAIL("..");
    SHOULD_FAIL("  ahoj  cau");
    SHOULD_FAIL("ahoj svete.");
    SHOULD_FAIL(".a.Ahoj");
    SHOULD_FAIL("a");
    SHOULD_FAIL("aB");
    SHOULD_FAIL("Ahoj..");
    SHOULD_FAIL("[!");
    SHOULD_FAIL("8 Ahoj");
    SHOULD_PASS("\n");

    TEST_DONE;
}

TEST(allSentencesAreClosed) {
    TEST_FUNCTION(allSentencesAreClosed);

    SHOULD_PASS("Ahoj. ");
    SHOULD_PASS("ahoj.");
    SHOULD_PASS("{}");
    SHOULD_PASS(".a.a.");
    SHOULD_PASS("Dlhy test   .Ci   ? ");
    SHOULD_PASS("...");
    SHOULD_PASS("?!.[]{}");
    SHOULD_PASS("?!a.[]{}");
    SHOULD_PASS("?!Ah Asd Asd. Asff Adf.");

    SHOULD_FAIL("ahoj");
    SHOULD_FAIL("Ahoj.{}A");
    SHOULD_FAIL("Hello. bye");
    SHOULD_FAIL(".cau");
    SHOULD_FAIL("?[a");
    SHOULD_FAIL("?!.a[]{}");


    TEST_DONE;
}

TEST(noSpaceBeforeSeparators) {
    TEST_FUNCTION(noSpaceBeforeSeparators);

    SHOULD_PASS("Ahoj.");
    SHOULD_PASS("Bezva,ako sa mas?");
    SHOULD_PASS("");
    SHOULD_PASS(",.!?");
    SHOULD_PASS("lol, mam, nemas!, chod prec.");

    SHOULD_FAIL("ahoj , ako?");
    SHOULD_FAIL(" .");
    SHOULD_FAIL("ahoj. ako sa mas ?");

    TEST_DONE;
}

TEST(spaceAfterSeparators) {
    TEST_FUNCTION(spaceAfterSeparators);

    SHOULD_PASS("hello, world");
    SHOULD_PASS("");
    SHOULD_PASS(", , ");
    SHOULD_PASS("Ahoj!");
    SHOULD_PASS("Ahoj! !");


    SHOULD_FAIL(",,");
    SHOULD_FAIL(",, ");
    SHOULD_FAIL("!ahoj");
    SHOULD_FAIL("ahoj!a");
    SHOULD_FAIL("hello,world");

    TEST_DONE;
}

TEST(properParenthesesPairing) {
    TEST_FUNCTION(properParenthesesPairing);

    SHOULD_PASS("");
    SHOULD_PASS("(hello)");
    SHOULD_PASS("[]0kwsdofjweoú");
    SHOULD_PASS("Hello world (again)!");
    SHOULD_PASS("Hello, bitche(s.How are u all?).");

    SHOULD_FAIL("]");
    SHOULD_FAIL("(");
    SHOULD_FAIL("{");
    SHOULD_FAIL("[");
    SHOULD_FAIL("]ssas[");
    SHOULD_FAIL(":(");
    SHOULD_FAIL("hello, (tiny (puny)) world!");
    SHOULD_FAIL("(ahojte(cau))");
    SHOULD_FAIL("{]");
    SHOULD_FAIL("cauko (}");
    SHOULD_FAIL("a(ejief");

    TEST_DONE;
}

TEST(noParenthesesSentenceCrossing) {
    TEST_FUNCTION(noParenthesesSentenceCrossing);

    SHOULD_FAIL("(nespravne.)(a");
    SHOULD_FAIL("(nespravne)");
    SHOULD_FAIL("Spravne.(ahoj) Uzatvorkovanie.");
    SHOULD_FAIL("First (sentence. Second) sentence.");
    SHOULD_FAIL("(First sentence. Second) sentence.");
    SHOULD_FAIL("Ahoj(.)");

    SHOULD_PASS("(First sentence. Second sentence.) Third sentence.");
    SHOULD_PASS("(First sentence. Second sentence.) Third (in) sentence.");
    SHOULD_PASS("First (inside) sentence.");
    SHOULD_PASS("()");
    SHOULD_PASS("");
    SHOULD_PASS("(.)");
    SHOULD_PASS("(spravne.)");

    TEST_DONE;
}

TEST(noSpaceBeforeClosingOrAfterOpeningApostrophe) {
    TEST_FUNCTION(noSpaceBeforeClosingOrAfterOpeningApostrophe);

    SHOULD_FAIL("\"now this is\" incorrect.\"");
    SHOULD_PASS("Hello1 \"kid\"");
    SHOULD_PASS("");
    SHOULD_PASS("A");
    SHOULD_PASS("\"\"");
    SHOULD_PASS("\"correct apostrophe\"");
    SHOULD_PASS("\"still.correct.ap\"");

    SHOULD_FAIL("\"now this is incorrect. \"");
    SHOULD_FAIL("\" also this.\"");
    SHOULD_FAIL("Hello2 \" kid\"");
    SHOULD_FAIL("Hello3 \"kid \"");
    SHOULD_FAIL("\"");

    TEST_DONE;
}

/****  YOU SHOULD NOT EDIT ANYTHING BELOW THIS LINE  *************************/

int main(void)
{
    TEST_RUN(sentenceStartsWithUppercase);
    TEST_RUN(noSpaceBeforeSeparators);
    TEST_RUN(noTwoConsecutiveSpaces);
    TEST_RUN(spaceAfterSeparators);
    TEST_RUN(properParenthesesPairing);
    TEST_RUN(noParenthesesSentenceCrossing);
    TEST_RUN(allSentencesAreClosed);
    TEST_RUN(noSpaceBeforeClosingOrAfterOpeningApostrophe);
    return 0;
}
