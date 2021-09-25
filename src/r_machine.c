/**
 * Copyright (c) 2015 - 2021 YIYAS
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include <r_machine.h>
#include <libal/dict.h>

int r_init() {
    return al_dict_init(100);
}

void r_destroy() {
    al_dict_destroy();
}
