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
    int rc = r_init();
    TEST_ASSERT_EQUAL(0, rc);
}

void tearDown(void) {
    r_destroy();
}

static void parse_and_print_eq(const char *express, const char *expect) {
    struct r_error *err = NULL;
    struct r_logic_sentence *stc = r_sentence_parse(express, &err);

    TEST_ASSERT_NOT_NULL(stc);
    TEST_ASSERT_NULL(err);

    char *str = NULL;
    int rc = r_sentence_print(&str, stc);

    TEST_ASSERT_EQUAL(0, rc);
    TEST_ASSERT_EQUAL_STRING(expect, str);

    r_sentence_destroy(stc);
    free(str);
}

static void test_single_operator() {
    parse_and_print_eq("true", "true");
    parse_and_print_eq("false", "false");

    parse_and_print_eq("A", "A");
}

static void test_and() {
    parse_and_print_eq("true and false", "true and false");
    parse_and_print_eq("false and A", "false and A");
    parse_and_print_eq("A and BX", "A and BX");
}

static void test_or() {
    parse_and_print_eq("true or false", "true or false");
    parse_and_print_eq("false or A", "false or A");
    parse_and_print_eq("A or BX", "A or BX");
}

static void test_not() {
    parse_and_print_eq("!false", "!false");
    parse_and_print_eq("!A", "!A");
}

static void test_mix() {
    parse_and_print_eq("A and B and C", "A and B and C");
    parse_and_print_eq("A and B or C", "A and B or C");
    parse_and_print_eq("A or B or C", "A or B or C");

    parse_and_print_eq("A or !B", "A or !B");
    parse_and_print_eq("!(A or B)", "!(A or B)");
    parse_and_print_eq("(A or B) and C", "(A or B) and C");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_single_operator);
    RUN_TEST(test_and);
    RUN_TEST(test_or);
    RUN_TEST(test_not);
    RUN_TEST(test_mix);
    return UNITY_END();
}
