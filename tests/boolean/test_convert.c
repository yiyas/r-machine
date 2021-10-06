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

#include "../unity/unity.h"
#include "r_machine.h"

void setUp(void) {
    int rc = r_init();
    TEST_ASSERT_EQUAL(0, rc);
}

void tearDown(void) {
    r_destroy();
}

static void test_not_not() {
    struct r_logic_sentence *st, *from, *to, *rt;

    from = r_sentence_parse("!!A", NULL);
    TEST_ASSERT_NOT_NULL(from);

    to = r_sentence_parse("A", NULL);
    TEST_ASSERT_NOT_NULL(to);

    st = r_sentence_parse("!!B", NULL);
    TEST_ASSERT_NOT_NULL(st);

    rt = r_convert(st, from, to, NULL);
    TEST_ASSERT_NOT_NULL(rt);

    char *str = NULL;
    int rc = r_sentence_print(&str, rt);
    TEST_ASSERT_EQUAL(0, rc);

    TEST_ASSERT_EQUAL_STRING("B", str);

    free(str);

    r_sentence_destroy(st);
    r_sentence_destroy(from);
    r_sentence_destroy(to);
    r_sentence_destroy(rt);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_not_not);
    return UNITY_END();
}
