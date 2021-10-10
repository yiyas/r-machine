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

static void assert_convert_succ(const char *from, const char *to, const char *orig, const char *expect_rlt) {
    struct r_logic_sentence *from_st, *to_st, *orig_st, *rlt_st;

    from_st = r_sentence_parse(from, NULL);
    TEST_ASSERT_NOT_NULL(from_st);

    to_st = r_sentence_parse(to, NULL);
    TEST_ASSERT_NOT_NULL(to_st);

    orig_st = r_sentence_parse(orig, NULL);
    TEST_ASSERT_NOT_NULL(orig_st);

    rlt_st = r_convert(orig_st, from_st, to_st, NULL);
    TEST_ASSERT_NOT_NULL(rlt_st);

    char *str = NULL;
    int rc = r_sentence_print(&str, rlt_st);
    TEST_ASSERT_EQUAL(0, rc);

    TEST_ASSERT_EQUAL_STRING(expect_rlt, str);

    free(str);

    r_sentence_destroy(from_st);
    r_sentence_destroy(to_st);
    r_sentence_destroy(orig_st);
    r_sentence_destroy(rlt_st);
}

static void test_commutative_law() {
    assert_convert_succ("A and B", "B and A", "x and true", "true and x");
    assert_convert_succ("A and B", "B and A", "x and (y or z)", "(y or z) and x");

    assert_convert_succ("A or B", "B or A", "x or true", "true or x");
    assert_convert_succ("A or B", "B or A", "x or (y or z)", "y or z or x");
}

static void test_combination_law() {
    assert_convert_succ("A and B and C", "A and (B and C)", "x and y and z", "x and (y and z)");
    assert_convert_succ("A and B and C", "A and (B and C)", "(x or x) and y and z", "(x or x) and (y and z)");

    assert_convert_succ("A or B or C", "A or (B or C)", "x or y or z", "x or (y or z)");
    assert_convert_succ("A or B or C", "A or (B or C)", "(x and x) or y or z", "x and x or (y or z)");

}

static void test_double_negation_elimination() {
    assert_convert_succ("!!A", "A", "!!B", "B");
    assert_convert_succ("!!A", "A", "!!(A and B)", "A and B");
    assert_convert_succ("!!A", "A", "!!false", "false");
}

static void test_de_morgan_law() {
    assert_convert_succ("!(A and B)", "!A or !B", "!((a or b) and c)", "!(a or b) or !c");
    assert_convert_succ("!A or !B", "!(A and B)", "!(a or b) or !c", "!((a or b) and c)");

    assert_convert_succ("!(A or B)", "!A and !B", "!(a or b and c)", "!a and !(b and c)");
    assert_convert_succ("!A and !B", "!(A or B)", "!a and !(b and c)", "!(a or b and c)");
}

static void test_distributive_law() {
    assert_convert_succ("A and (B or C)", "(A and B) or (A and C)", "(a or c) and (x or x and y)", "(a or c) and x or (a or c) and (x and y)");
    assert_convert_succ("(A and B) or (A and C)", "A and (B or C)", "(a or c) and x or (a or c) and (x and y)", "(a or c) and (x or x and y)");

    assert_convert_succ("A or (B and C)", "(A or B) and (A or C)", "(a or c) or (x and (x and y))", "(a or c or x) and (a or c or x and y)");
    assert_convert_succ("(A or B) and (A or C)", "A or (B and C)", "((a or c) or x) and ((a or c) or (x and y))", "a or c or x and (x and y)");
}

static void test_xor_define() {
    assert_convert_succ("A xor B", "(A and !B) or (!A and B)", "x xor y", "x and !y or !x and y");
    assert_convert_succ("(A and !B) or (!A and B)", "A xor B", "x and !y or !x and y", "x xor y");
}

static void test_if_define() {
    assert_convert_succ("A=>B", "!A or B", "x=>y", "!x or y");
    assert_convert_succ("!A or B", "A=>B", "!x or y", "x => y");
}

static void test_iff_define() {
    assert_convert_succ("A<=>B", "A=>B and B=>A", "x<=>y", "x => y and y => x");
    assert_convert_succ("A=>B and B=>A", "A<=>B", "x=>y and y=>x", "x <=> y");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_commutative_law);
    RUN_TEST(test_combination_law);
    RUN_TEST(test_double_negation_elimination);
    RUN_TEST(test_de_morgan_law);
    RUN_TEST(test_distributive_law);
    RUN_TEST(test_xor_define);
    RUN_TEST(test_if_define);
    RUN_TEST(test_iff_define);
    return UNITY_END();
}
