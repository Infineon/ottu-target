#include <stdbool.h>
#include "ottunity.h"

/* Boolean checks group */
OTTUNITY_GROUP(module_a);

/**
 * No setup and teardown functions needed.
 * Their definition is not required.
 * It will run the default empty function.
 */

OTTUNITY_GROUP_TEST(module_a, always_true)
{
    TEST_ASSERT_TRUE_MESSAGE(true, "This should always be true");
}

OTTUNITY_GROUP_TEST(module_a, always_false)
{
    TEST_ASSERT_FALSE_MESSAGE(false, "This should always be false");
}
