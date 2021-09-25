/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "unity/unity.h"
#include "r_machine.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_single_operator() {
    struct r_error *err = NULL;
    struct r_logic_sentence *stc = r_sentence_parse("true", &err);

    TEST_ASSERT_NOT_NULL(stc);
    TEST_ASSERT_NULL(err);

    char *str = NULL;
    int rc = r_sentence_print(&str, stc);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL_STRING("true", str);

    r_sentence_destroy(stc);
    free(str);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_single_operator);
    return UNITY_END();
}
