#include <stdbool.h>
#include "ottunity.h"

OTTUNITY_TEST(is_null_pointer) {
    int *ptr = NULL;

    TEST_ASSERT_NULL_MESSAGE(ptr, "Pointer should be null");
}

OTTUNITY_TEST(does_truth_exist) {
    bool truth = false;

    TEST_ASSERT_FALSE(truth);
}

OTTUNITY_RUN();
