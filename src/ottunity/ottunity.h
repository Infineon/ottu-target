#ifndef __OTTUNITY_H
#define __OTTUNITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ottunity_priv.h"

/**
 * ---------------------------------------------------
 * ----------------- Ottunity basic API ----------------
 * ---------------------------------------------------
 *
 * Basic API when no grouping is required.
 *
 * The definition of setup and tear down functions is
 * optional. An empty function will run when not provided.
 *
 * Define every test and finally call the ottunity runner:
 *
 *      OTTUNITY_TEST(test_1);
 *      ...
 *      OTTUNITY_TEST(test_n);
 *
 *      OTTUNITY_RUN();
 *
 * The tests will be added to a pre-instantiated group. The
 * group name will be automatically set to the file name
 * where the OTTUNITY_RUN() main function is called.
 */

#define OTTUNITY_TEST_SETUP() \
        OTTUNITY_GROUP_TEST_SETUP(auto)

#define OTTUNITY_TEST_TEAR_DOWN() \
        OTTUNITY_GROUP_TEST_TEAR_DOWN(auto)

#define OTTUNITY_TEST(name) \
        OTTUNITY_GROUP_TEST(auto, name)

/**
 * Main ottunity runner function
 *
 * This is a generic reference implementation. The
 * environment specific configuration can be implemented via
 * the OTTUNITY_ENV_SETUP() macro.
 *
 * When this default implementation isn't sufficient or
 * appropriate, define a custom OTTUNITY_RUN(), and call
 * OTTUNITY_SUITE_RUNNER() inside and return its result.
 */
#ifndef OTTUNITY_RUN
#define OTTUNITY_RUN() \
        int main(void) { \
            OTTUNITY_ENV_SETUP(); \
            return OTTUNITY_SUITE_RUNNER(); \
        }
#endif

/**
 * ---------------------------------------------------
 * ----------------- Ottunity group API ----------------
 * ---------------------------------------------------
 *
 * Group API for organizing tests with group-specific setup
 * and tear down functions.
 *
 * Setup and tear down functions are optional. Define them
 * only if your tests need them.
 *
 */

#define OTTUNITY_GROUP(group) \
        static ottunity_group_t ottunity_group_##group = \
            OTTUNITY_GROUP_INIT(group); \
        OTTUNITY_SUITE_REGISTER_GROUP(group);

#define OTTUNITY_GROUP_TEST_SETUP(group) \
        static void ottunity_group_##group##_test_setup(void); \
        OTTUNITY_GROUP_SET_PARAM(group, setup, \
                ottunity_group_##group##_test_setup); \
        static void ottunity_group_##group##_test_setup(void)

#define OTTUNITY_GROUP_TEST_TEAR_DOWN(group) \
        static void ottunity_group_##group##_test_tear_down(void); \
        OTTUNITY_GROUP_SET_PARAM(group, tear_down, \
                ottunity_group_##group##_test_tear_down); \
        static void ottunity_group_##group##_test_tear_down(void)

#define OTTUNITY_GROUP_TEST(group, name) \
        static void ottunity_test_##group##_##name##_(void); \
        OTTUNITY_GROUP_TEST_SET_RUNNER(group, name); \
        OTTUNITY_GROUP_REGISTER_TEST(group, name); \
        static void ottunity_test_##group##_##name##_(void)

#ifdef __cplusplus
}
#endif

#endif /* __OTTUNITY_H */
