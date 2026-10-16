#include <stdbool.h>
#include "ottunity.h"

/* Boolean checks group */
OTTUNITY_GROUP(boolean_checks);

/**
 * No setup and teardown functions needed.
 * Their definition is not required.
 * It will run the default empty function.
 */

OTTUNITY_GROUP_TEST(boolean_checks, always_true)
{
    TEST_ASSERT_TRUE_MESSAGE(true, "This should always be true");
}

OTTUNITY_GROUP_TEST(boolean_checks, always_false)
{
    TEST_ASSERT_FALSE_MESSAGE(false, "This should always be false");
}

/* Character checks group */

OTTUNITY_GROUP(chars_checks);

char c;
char d;

OTTUNITY_GROUP_TEST_SETUP(chars_checks)
{
    c = 'c';
    d = 'd';
}

OTTUNITY_GROUP_TEST(chars_checks, chars_not_equal)
{
    TEST_ASSERT_NOT_EQUAL_MESSAGE(c, d, "Characters c and d should not be equal");
}

OTTUNITY_GROUP_TEST(chars_checks, char_equal)
{
    TEST_ASSERT_EQUAL_MESSAGE(c, 'c', "Character c should be equal to 'c'");
}

OTTUNITY_GROUP_TEST(chars_checks, char_var_mutable)
{
    char old_d = d;
    d = 'z';

    TEST_ASSERT_NOT_EQUAL_MESSAGE(d, old_d, "Character d should have changed after assignment");
}

OTTUNITY_RUN();
