/*
 * This file is generated by gen_cache_asm_files.py.
 * MODIFICATIONS WILL THUS BE OVERWRITTEN.
 */

#ifndef HEADER_L1_ASM_H
#define HEADER_L1_ASM_H

#include "asm.h"
#include "cache.h"
#include "device_conf.h"


// Traverse cache sets in reverse order for minimal cache impact
static inline cacheline *asm_l1_probe_cacheset(cacheline *curr_cl) {
    cacheline *next_cl;

    start_timer();
    asm volatile(
        "mov 8(%[curr_cl]), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %%rax \n\t"
        "mov 8(%%rax), %%rcx \n\t"
        "mov 8(%%rcx), %[curr_cl_out] \n\t"
        "mov 8(%[curr_cl_out]), %[next_cl_out] \n\t"
        : [next_cl_out] "=rm" (next_cl), [curr_cl_out] "=rm" (curr_cl)
        : [curr_cl] "r" (curr_cl)
        : "%rax", "%rcx"
    );
    stop_timer(&(curr_cl->time_msrmt));

    return next_cl;
}

#endif // HEADER_L1_ASM_H