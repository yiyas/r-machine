/**
 * Copyright (c) 2015 - 2017 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef R_MACHINE_H_
#define R_MACHINE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return 0: entail, 1: not entail, -1: error
 */
int r_pl_tt_entail(const char **kb, uint32_t kb_size, const char *q);

#ifdef __cplusplus
}
#endif

#endif /* R_MACHINE_H_ */
