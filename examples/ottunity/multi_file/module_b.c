#include "ottunity.h"

OTTUNITY_GROUP(module_b);

char c;
char d;

OTTUNITY_GROUP_TEST_SETUP(module_b) {
    c = 'c';
    d = 'd';
}

OTTUNITY_GROUP_TEST(module_b, chars_not_equal) {
    TEST_ASSERT_NOT_EQUAL_MESSAGE(c, d, "Characters c and d should not be equal");
}

OTTUNITY_GROUP_TEST(module_b, char_equal) {
    TEST_ASSERT_EQUAL_MESSAGE(c, 'c', "Character c should be equal to 'c'");
}

OTTUNITY_GROUP_TEST(module_b, char_var_mutable) {
    char old_d = d;
    d = 'z';

    TEST_ASSERT_NOT_EQUAL_MESSAGE(d, old_d, "Character d should have changed after assignment");
}
