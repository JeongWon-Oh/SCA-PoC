/*
 * This file is part of the CacheSC library (https://github.com/Miro-H/CacheSC),
 * which implements Prime+Probe attacks on virtually and physically indexed
 * caches.
 *
 * Copyright (C) 2020  Miro Haller
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: miro.haller@alumni.ethz.ch
 *
 * Short description of this file:
 * This file demonstrates the usage of CacheSC on the artificial example of a
 * single cache line eviction between prime and probe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cachesc.h>
#include "gem5/m5ops.h"

/*
 * Configure side-channel attack
 */

//#define GEM5

// Pin process to a CPU. To reduce noise, this CPU can be isolated.
#define CPU_NUMBER 1
// Set which is targeted for the eviction that this demo measures
#define TARGET_SET 3

// This demo can be run on L1 or L2, uncomment the respective macros below

// Uncomment for L1 attack
#define TARGET_CACHE L1
#define MSRMTS_PER_SAMPLE L1_SETS
#define PRIME prime

// Uncomment for L2 attack
//#define TARGET_CACHE L2
//#define MSRMTS_PER_SAMPLE L2_SETS
//#define PRIME prime_rev

// local functions
void usage(const char *prog);

int main(int argc, char **argv) {
    uint32_t sample_cnt = -1;

    if (argc == 2)
        sample_cnt = atoi(argv[1]);
    if (argc != 2 || sample_cnt < 0)
        usage(argv[0]);


    /*
     * Initial preparation
     */
    PRINT_LINE("Initial attacker preparation\n");
    PRINT_LINE("Number of samples: %d\n", sample_cnt);
    PRINT_LINE("Measurements per sample: %d\n", MSRMTS_PER_SAMPLE);
    //printf("cacheline pointer size: %ld\n", sizeof(cacheline*));
    // Get a cache context object containing the dimensions of the attacked
    // cache.
    cache_ctx *ctx = get_cache_ctx(TARGET_CACHE);
    // Prepare the Prime+Probe data structure. For unprivileged L2 attacks,
    // this can take a while.
    cacheline *cache_ds = prepare_cache_ds(ctx);
    
    print_cache_ctx(ctx);
    
    //int iii = 0; int jjj = 0; int prevcset = 64;
    //for(cacheline *curr = cache_ds->next; iii<4; curr = curr->next) {
    //    //printf("iter:%d | %d\n", jjj++, iii);
    //    print_cacheline(curr);
    //    if(curr->cache_set != prevcset) {
    //        
    //        iii++;
    //    }
    //    prevcset = curr->cache_set;
    //}

    // Prepare an array to store the time measurements
    size_t res_size = sample_cnt * MSRMTS_PER_SAMPLE * sizeof(time_type);
    time_type *res  = (time_type *) malloc(res_size);
    assert(res);
    memset(res, 0, res_size);

    // Prepare victim, which we later use to access a cache line in the
    // targeted set.
    cacheline *victim_ptr = prepare_victim(ctx, TARGET_SET);
    print_cacheline(cache_ds);
    print_cacheline(victim_ptr);
    //printf("-----\n");
    //printf("hello flsdkajflksdaj\nflkajdlfdsjkl\n");
    //printf("one more line\n");
    //print_cache_ds(cache_ds);
    //printf("-----\n");
    //print_cache_ds(victim_ptr);
    PRINT_LINE("Legend: target set: %d\n", TARGET_SET);

    // Pin process to a CPU
    //pin_to_cpu(CPU_NUMBER);

    uint32_t i;
    uint32_t *curr_res      = res;
    cacheline *curr_head    = cache_ds;
    cacheline *next_head;

    prepare_measurement();

    /*
     * Make baseline measurements for normalisation (optional)
     */
    //#ifdef NORMALIZE
    //for (i = 0; i < sample_cnt; ++i) {
    //    curr_head = PRIME(curr_head);
    //    next_head = probe(TARGET_CACHE, curr_head);
    //
    //    get_msrmts_for_all_set(curr_head, curr_res);
    //    curr_head = next_head;
    //    curr_res += MSRMTS_PER_SAMPLE;
    //}
    //
    //PRINT_LINE("Output cache set access baseline data\n");
    //print_results(res, sample_cnt, MSRMTS_PER_SAMPLE);
    //
    //// reset changes
    //memset(res, 0, res_size);
    //curr_res    = res;
    //curr_head   = cache_ds;
    //#endif


    /*
     * Start attacking for "sample_cnt" rounds
     */
    print_banner("Start cache attack(s)");

    prepare_measurement();

    
    for (i = 0; i < sample_cnt; ++i) {
        //m5_dump_reset_stats(0, 0);
        
        curr_head = PRIME(curr_head);
        
        
        // Access cache line in target cache set
        victim(victim_ptr);
        #ifdef GEM5
        m5_work_begin(0, 0);
        #endif
        next_head = probe(TARGET_CACHE, curr_head);
        #ifdef GEM5
            m5_work_end(0, 0);
        #endif        
        get_msrmts_for_all_set(curr_head, curr_res);
        curr_head = next_head;
        curr_res += MSRMTS_PER_SAMPLE;
        
    }
    

    print_banner("Stop cache attack(s)");

    //printf("-----\n");
    //print_cache_ds(cache_ds);
    //printf("-----\n");
    //print_cache_ds(victim_ptr);

    /*
     * Print output
     */
    PRINT_LINE("Output cache attack data\n");
    print_results(res, sample_cnt, MSRMTS_PER_SAMPLE);

    //print_cache_ds(cache_ds);

    /*
     * Cleanup
     */
    printf("flag1\n");
    free(res);
    printf("flag2\n");
    release_cache_ds(ctx, cache_ds);
    printf("flag3\n");
    release_victim(ctx, victim_ptr);
    printf("flag4\n");
    release_cache_ctx(ctx);
    printf("flag5\n");

    return EXIT_SUCCESS;
}

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <samples>\n", prog);
    exit(EXIT_FAILURE);
}
