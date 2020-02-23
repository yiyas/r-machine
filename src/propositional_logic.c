/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "r_machine.h"

#define PL_ENTAIL      0
#define PL_NOTENTAIL   1
#define PL_ERROR      -1

struct pl_clause {
    enum {
        PL_AND, PL_OR, PL_NOT, PL_SYMBOL
    } op;
    union {
        struct pl_clause *sub_clauses[2];
        int symbol_id;
    } operand;
};

void pl_clause_free(struct pl_clause *clause) {

}

static int parse_kb(const char **kb, uint32_t kb_size, struct pl_clause **clause, char ***symbols,
        uint32_t *symbol_size) {
    return 0;
}

static int parse_clause_with_symbols(const char *exp, struct pl_clause **clause, char *const *symbols,
        uint32_t symbol_size) {
    return 0;
}

static void symbols_free(char **symbols, uint32_t symbol_size) {
    uint32_t i;

    if(!symbols) {
        return;
    }

    for (i = 0; i < symbol_size; i++) {
        free(symbols[i]);
    }
}

int r_pl_tt_entail(const char **kb, uint32_t kb_size, const char *q) {
    int rt;
    struct pl_clause *kb_clause = NULL, *q_clause = NULL;
    char **symbols = NULL;
    uint32_t symbol_size;

    rt = parse_kb(kb, kb_size, &kb_clause, &symbols, &symbol_size);
    if (rt) {
        rt = PL_ERROR;
        goto clean;
    }

    rt = parse_clause_with_symbols(q, &q_clause, symbols, symbol_size);
    if (rt) {
        rt = PL_ERROR;
        goto clean;
    }

    clean:
    pl_clause_free(kb_clause);
    pl_clause_free(q_clause);
    symbols_free(symbols, symbol_size);
    return rt;
}
