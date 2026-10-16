#include <stdbool.h>
#include "ottunity.h"

TEST_GROUP(numeric_check);

TEST_SETUP(numeric_check) {
    /* no setup */
}

TEST_TEAR_DOWN(numeric_check) {
    /* no tear down*/
}

TEST(numeric_check, test_greater_than) {
    TEST_ASSERT_GREATER_THAN_INT_MESSAGE(5, 10, "10 should be greater than 5");
}

TEST(numeric_check, test_less_than) {
    TEST_ASSERT_LESS_THAN_INT_MESSAGE(3, 1, "1 should be less than 3");
}

TEST_GROUP_RUNNER(numeric_check) {
    RUN_TEST_CASE(numeric_check, test_greater_than);
    RUN_TEST_CASE(numeric_check, test_less_than);
}

void run_all_tests() {
    RUN_TEST_GROUP(numeric_check);
};

void main() {
    const int argc = 2;
    const char *argv[] = { "", "-v" };
    UnityMain(argc, argv, run_all_tests);
}
