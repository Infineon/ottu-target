#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "ottunity.h"
#include "ottunity_priv.h"

/**
 * ---------------------------------------------------
 * ------------------- Ottunity suite ------------------
 * ---------------------------------------------------
 */

ottunity_suite_t ottunity_suite = {{NULL}, 0};

void ottunity_suite_register_group(ottunity_group_t *group) {
    if (ottunity_suite.group_count >= OTTUNITY_SUITE_GROUP_NUM_MAX) {
        return;
    }
    ottunity_suite.group_list[ottunity_suite.group_count++] = group;
}

static void ottunity_suite_runner(void) {
    for (uint32_t g = 0; g < ottunity_suite.group_count; g++) {
        for (uint32_t t = 0; t < ottunity_suite.group_list[g]->test_count; t++) {
            ottunity_suite.group_list[g]->test_list[t]->runner();
        }
    }
}

int ottunity_main_runner(void) {
    const int argc = 2;
    const char *argv[] = { "", "-v" };
    return UnityMain(argc, argv, ottunity_suite_runner);
}

/**
 * ---------------------------------------------------
 * ------------------- Ottunity group ------------------
 * ---------------------------------------------------
 */

void ottunity_group_register_test(ottunity_group_t *group, ottunity_test_t *test) {
    if (group->test_count > OTTUNITY_GROUP_TEST_NUM_MAX - 1) {
        return;
    }
    group->test_list[group->test_count++] = test;
}

static void ottunity_group_test_get_printable_name(const char *group, const char *name, char *test_printable_name) {
    snprintf(test_printable_name, OTTUNITY_ANY_STR_NAME_MAX_LEN, "TEST(%s, %s)", group, name);
}

void ottunity_group_test_set_runner(ottunity_group_t *group, ottunity_test_t *test, const char *file, unsigned int line) {
    char printable_name[OTTUNITY_ANY_STR_NAME_MAX_LEN] = {"\0"};
    ottunity_group_test_get_printable_name(group->name, test->name, printable_name);
    UnityTestRunner(group->setup,
        test->body,
        group->tear_down,
        printable_name,
        group->name,
        test->name,
        file, line);
}

static char ottunity_group_auto_name[OTTUNITY_ANY_STR_NAME_MAX_LEN] = {'\0'};
ottunity_group_t ottunity_group_auto = OTTUNITY_GROUP_INIT(ottunity);
__attribute__((constructor)) static void ottunity_suite_register_group_auto(void) {
    ottunity_suite_register_group(&ottunity_group_auto);
}

static void get_filename_without_ext(const char *filepath, char *filename_base);

void ottunity_group_auto_set_name_from_file(const char *file) {
    get_filename_without_ext(file, ottunity_group_auto_name);
    ottunity_group_auto.name = ottunity_group_auto_name;
}

/**
 * ---------------------------------------------------
 * --------------------- Utilities -------------------
 * ---------------------------------------------------
 */

/**
 * Empty function.
 *
 * The default setup and tear down functions are initialized
 * to use this function.
 */
void ottunity_do_nothing_func(void) {
}

static void get_filename_without_ext(const char *filepath, char *filename_base) {
    const char *filename = filepath;
    for (const char *character = filepath; *character != '\0'; character++) {
        if (*character == '/' || *character == '\\') {
            filename = character + 1;
        }
    }
    const char *dot = strrchr(filename, '.');
    size_t len = dot ? (size_t)(dot - filename) : strlen(filename);

    if (len >= OTTUNITY_ANY_STR_NAME_MAX_LEN) {
        len = OTTUNITY_ANY_STR_NAME_MAX_LEN - 1;
    }

    memcpy(filename_base, filename, len);
    filename_base[len] = '\0';
}
