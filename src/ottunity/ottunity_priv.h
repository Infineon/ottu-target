#ifndef __OTTUNITY_PRIV_H
#define __OTTUNITY_PRIV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "unity.h"
#include "unity_fixture.h"

#include "ottunity_cfg_dflt.h"

void ottunity_do_nothing_func(void);
typedef void (*ottunity_runner_func_t)(void);

/**
 * Ottunity consists of one test suite. This suite is a
 * collection of test groups. And each test group is a
 * collection of tests. A group implements a setup and tear
 * down a function will run before and after every test in
 * the group:
 *
 *           ottunity_suite
 *                ^
 *              1 |
 *                |
 *              N |
 *                v
 *           ottunity_group
 *                ^
 *              1 |
 *                |
 *              N |
 *                v
 *           ottunity_test
 *
 * The maximum number of groups for the suite is defined by
 * the macro OTTUNITY_SUITE_GROUP_NUM_MAX.
 *
 * The maximum number of tests for a group is defined by the
 * macro OTTUNITY_GROUP_TEST_NUM_MAX.
 *
 * These are defined in the "ottunity_cfg_dflt.h" file. Both can
 * be overridden in a custom "ottunity_config.h" file.
 */

/**
 * -------------- OTTUNITY TEST DEFINITIONS --------------
 */

typedef struct _ottunity_test_t {
    const char *name;
    ottunity_runner_func_t runner;
    ottunity_runner_func_t body;
} ottunity_test_t;

#define OTTUNITY_TEST_INIT(name, runner, body) { #name, runner, body }

/**
 * -------------- OTTUNITY GROUP DEFINITIONS --------------
 */

typedef struct _ottunity_group_t {
    const char *name;
    ottunity_runner_func_t setup;
    ottunity_runner_func_t tear_down;
    ottunity_test_t *test_list[OTTUNITY_GROUP_TEST_NUM_MAX];
    uint32_t test_count;
} ottunity_group_t;

void ottunity_group_register_test(ottunity_group_t *group, ottunity_test_t *test);
void ottunity_group_test_set_runner(ottunity_group_t *group, ottunity_test_t *test, const char *file, unsigned int line);

#define OTTUNITY_GROUP_INIT(name) { #name, ottunity_do_nothing_func, ottunity_do_nothing_func, {NULL}, 0 }

#define OTTUNITY_GROUP_SET_PARAM(group, param, value) \
        static void ottunity_group_##group##_set_##param(void) __attribute__((constructor)); \
        static void ottunity_group_##group##_set_##param(void) { \
            ottunity_group_##group.param = value; \
        }

#define OTTUNITY_GROUP_REGISTER_TEST(group, name) \
        static void ottunity_group_##group##_register_test_##name(void) __attribute__((constructor)); \
        static void ottunity_group_##group##_register_test_##name(void) { \
            ottunity_group_register_test(&ottunity_group_##group, &ottunity_test_##group##_##name); \
        }

#define OTTUNITY_GROUP_TEST_SET_RUNNER(group, name) \
        static void ottunity_test_##group##_##name##_runner(void); \
        static ottunity_test_t ottunity_test_##group##_##name = \
            OTTUNITY_TEST_INIT(name, ottunity_test_##group##_##name##_runner, ottunity_test_##group##_##name##_); \
        static void ottunity_test_##group##_##name##_runner(void) { \
            ottunity_group_test_set_runner(&ottunity_group_##group, &ottunity_test_##group##_##name, __FILE__, __LINE__); \
        }

/**
 * This pre-instantiated group will be used when no group is
 * defined explicitly by the user.
 *
 * The name of this group will be taken from the file where
 * the OTTUNITY_RUN() macro is called. */
extern ottunity_group_t ottunity_group_auto;
void ottunity_group_auto_set_name_from_file(const char *file);

/**
 * -------------- OTTUNITY SUITE DEFINITIONS --------------
 */

typedef struct _ottunity_suite {
    ottunity_group_t *group_list[OTTUNITY_SUITE_GROUP_NUM_MAX];
    uint32_t group_count;
}ottunity_suite_t;

extern ottunity_suite_t ottunity_suite;

void ottunity_suite_register_group(ottunity_group_t *group);
int ottunity_main_runner(void);

#define OTTUNITY_SUITE_REGISTER_GROUP(group) \
        static void ottunity_suite_register_group_##group(void) __attribute__((constructor)); \
        static void ottunity_suite_register_group_##group(void) { \
            ottunity_suite_register_group(&ottunity_group_##group); \
        }

#define OTTUNITY_SUITE_RUNNER() \
        (ottunity_group_auto_set_name_from_file(__FILE__), ottunity_main_runner())

#ifdef __cplusplus
}
#endif

#endif /* __OTTUNITY_PRIV_H */
