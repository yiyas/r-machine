/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef R_INTERNAL_H_
#define R_INTERNAL_H_

#include <r_machine.h>

#define LOG_DBG(fmt, args...) r_log("DBG", __FILE__, __LINE__, fmt, ##args)
#define LOG_INF(fmt, args...) r_log("INF", __FILE__, __LINE__, fmt, ##args)
#define LOG_WRN(fmt, args...) r_log("WRN", __FILE__, __LINE__, fmt, ##args)
#define LOG_ERR(fmt, args...) r_log("ERR", __FILE__, __LINE__, fmt, ##args)

void r_log(const char *level, const char *file, int line, const char *fmt, ...) __attribute__((format(printf, 4, 5)));

struct r_logic_sentence {
    R_SENTENCE_TYPE type;

    union {
        struct r_logic_sentence *two[2];
        struct r_logic_sentence *one;
        R_BOOLEAN value;
        const char *name;
    } data;
};


#endif /* R_INTERNAL_H_ */
