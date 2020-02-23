/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include <stddef.h>
#include <string.h>

#include "unity/unity.h"
#include "../src/r_machine.h"

static const char *TEST_KB[] = {
        "A\\/B\\/C",
        "A->!B/\\!C",
        "B->!A/\\!C",
        "C->!A/\\!B",

        "D\\/E\\/F",
        "D->!E/\\!F",
        "E->!D/\\!F",
        "F->!D/\\!E",

        "A->D",
        "B->E",
        "C->F"
};

void setUp(void) {
}

void tearDown(void) {
}

static void test_tt_entail() {
    int rt;

    rt = r_pl_tt_entail(TEST_KB, sizeof(TEST_KB) / sizeof(TEST_KB[0]), "!A/\\!B->C/\\F");
    TEST_ASSERT_EQUAL_INT(0, rt);

    rt = r_pl_tt_entail(TEST_KB, sizeof(TEST_KB) / sizeof(TEST_KB[0]), "A/\E");
    TEST_ASSERT_EQUAL_INT(1, rt);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_tt_entail);
    return UNITY_END();
}
