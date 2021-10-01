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
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_not_not);
    return UNITY_END();
}
