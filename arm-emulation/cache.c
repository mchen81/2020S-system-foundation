#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "cache.h"
#include "utils.h"

/* Initialize cache_direct_mapped */
void cache_direct_mapped_init(struct cache_direct_mapped *dm, int size){
    struct cache_entry *cache_array;

    /* Allocte size number of cache entries for cache_array */
    cache_array = (struct cache_entry *) \
        malloc_helper(sizeof(struct cache_entry) * size,
                      "cache_direct_mapped_init() : cache_entry");
    for(int i=0;i<size;i++){
        cache_array[i].valid = 0;
    }

    dm->cache_array = cache_array;
    dm->size = size;
    dm->refs = 0;
    dm->hits = 0;
    dm->misses = 0;
}

void cache_fully_associative_init(struct cache_fully_associative *fa, int size){
    struct cache_entry *cache_array;

    /* Allocte size number of cache entries for cache_array */
    cache_array = (struct cache_entry *) \
        malloc_helper(sizeof(struct cache_entry) * size,
                      "cache_fully_associative_init() : cache_entry");
    
    for(int i=0;i<size;i++){
        cache_array[i].valid = 0;
    }

    fa->cache_array = cache_array;
    fa->size = size;
    fa->refs = 0;
    fa->hits = 0;
    fa->misses = 0;
}

void cache_set_associative_init(struct cache_set_associative *sa, int size){
    struct cache_entry *cache_array;

    /* Allocte size number of cache entries for cache_array */
    cache_array = (struct cache_entry *) \
        malloc_helper(sizeof(struct cache_entry) * size,
                      "cache_set_associative_init : cache_entry");

    for(int i=0;i<size;i++){
        cache_array[i].valid = 0;
    }

    sa->cache_array = cache_array;
    sa->size = size;
    sa->refs = 0;
    sa->hits = 0;
    sa->misses = 0;
}

void cache_direct_mapped_free(struct cache_direct_mapped *dm){
    free(dm->cache_array);
}

void cache_fully_associative_free(struct cache_fully_associative *fa){
    free(fa->cache_array);
}

void cache_set_associative_free(struct cache_set_associative *sa){
    free(sa->cache_array);
}

void cache_direct_mapped_stats(struct cache_direct_mapped *dm){
    int refs = dm->refs;
    if(refs == 0){
        refs = 1;
    }
    int hits = dm->hits;
    int misses = dm->misses;
    float hits_ratio = hits / (float)refs * 100.0;
    float misses_ratio = misses / (float)refs * 100.0;
    
    printf("Size %4d: refs = %5d hits = %5d (%.2f%) misses = %5d (%.2f%)\n",
            dm->size, dm->refs, hits, hits_ratio, misses, misses_ratio);
}

void cache_fully_associative_stats(struct cache_fully_associative *fa){
    int refs = fa->refs;
    if(refs == 0){
        refs = 1;
    }
    int hits = fa->hits;
    int misses = fa->misses;
    float hits_ratio = hits / (float)refs * 100.0;
    float misses_ratio = misses / (float)refs * 100.0;

    printf("Size %4d: refs = %5d hits = %5d (%.2f%) misses = %5d (%.2f%)\n",
            fa->size, fa->refs, hits, hits_ratio, misses, misses_ratio);
}

void cache_set_associative_stats(struct cache_set_associative *sa){
    int refs = sa->refs;
    if(refs == 0){
        refs = 1;
    }
    int hits = sa->hits;
    int misses = sa->misses;
    float hits_ratio = hits / (float)refs * 100.0;
    float misses_ratio = misses / (float)refs * 100.0;
    
    printf("Size %4d: refs = %5d hits = %5d (%.2f%) misses = %5d (%.2f%)\n",
            sa->size, sa->refs, hits, hits_ratio, misses, misses_ratio);
}

/* Cache Direct Mapped Lookup */
unsigned int cache_lookup_direct_mapped(struct cache_direct_mapped *dm,
                                        unsigned int addr){
    unsigned int tag;
    unsigned int index;
    /* The index_mask is used extract the index number from the addr */
    unsigned int index_mask;
    /* The index_bits is the number of bits need for the index, this is
     * used to extract the tag from the addr.
     */
    unsigned int index_bits = 0;
    unsigned int dm_size = dm->size;
    struct cache_entry *ce;

    /* Extract the index */
    index_mask = (dm->size) - 1;
    index = (addr >> 2) & index_mask;

    /* Simple log_2(n) calculation */
    while (dm_size > 1) {
        index_bits += 1;
        dm_size >>= 1;
    }

    /* Extract the tag */
    tag = addr >> (index_bits + 2);

    /* Lookup the cache_entry */
    ce = &dm->cache_array[index];

    /* Determine if addr is in the cache or not. */
    dm->refs += 1;
    if (ce->valid && (ce->tag == tag)) {
        /* hit */
        dm->hits += 1;
    } else {
        /* miss */
        dm->misses += 1;
        ce->valid = 1;
        ce->tag = tag;
        ce->data = *((unsigned int *) addr);
    }
    return ce->data;
}

/* Cache Fully Associative Lookup*/
unsigned int cache_lookup_fully_associative(struct cache_fully_associative *fa, unsigned int addr){
    unsigned int tag;
    unsigned int fa_size = fa->size;
    struct cache_entry *ce;

    tag = addr >> 2;
    fa->refs += 1;

    int i;
    for(i = 0; i < fa_size; i++){
        ce = &fa->cache_array[i];
        if(ce->valid){
            if(ce->tag == tag){
                fa->hits += 1;
                break;
            }
            if(i== fa_size - 1){ // cache is full, randomly replace one
                fa->misses += 1;
                srand(time(0));
                int rand_index = rand() % (fa_size - 1);
                ce = &fa->cache_array[i];
            }
        }else{
            // miss
            fa->misses += 1;
            ce->valid = 1;
            ce->tag = tag;
            ce->data = *((unsigned int *) addr);
            break;
        }
    }
    return ce->data;
}

unsigned int cache_lookup_set_associative(struct cache_set_associative *sa, unsigned int addr){
    unsigned int associate_size = 4;

    unsigned int tag;
    unsigned int index;
    unsigned int index_mask;
    unsigned int index_bits = 0;
    unsigned int set_size = sa->size / associate_size;
    struct cache_entry *ce;

    index_mask = set_size - 1;
    index = (addr >> 2) & index_mask;

    while (set_size > 1) {
        index_bits += 1;
        set_size >>= 1;
    }

    tag = addr >> (index_bits + 2);
    sa->refs += 1;

    for(int i = 0; i < associate_size; i++){
        ce = &sa->cache_array[index * associate_size + i];
        if(ce->valid && (ce->tag == tag)){
            sa->hits += 1;
            break;
        }else if(ce->valid && (i == associate_size - 1)){ //miss and cache block is full
            sa->misses += 1;
            srand(time(0));
            int rand_num = rand() % associate_size;
            ce = &sa->cache_array[index * associate_size + rand_num];
            ce->tag = tag;
            ce->data = *((unsigned int *) addr);
            break;
        }else if(!ce->valid){ // miss and cache block is not full
            sa->misses += 1;
            ce->valid = 1;
            ce->tag = tag;
            ce->data = *((unsigned int *)addr);
            break;
        }
    }
    return ce->data;
}

/* Initialize cache data structures */
void cache_state_init(struct cache_state *cstate) {
    struct cache_direct_mapped *dm_array;
    struct cache_fully_associative *fa_array;
    struct cache_set_associative *sa_array;

    int sizes[] = {8, 32, 128, 1024};
    int sizes_length = 4;

    dm_array = (struct cache_direct_mapped *)
        malloc_helper(sizeof(struct cache_direct_mapped) * sizes_length,
                      "cache_state_init() : cache_direct_mapped");
    
    fa_array = (struct cache_fully_associative *)
        malloc_helper(sizeof(struct cache_fully_associative) * sizes_length,
                      "cache_state_init() : cache_fully_associative");
    
    sa_array = (struct cache_set_associative * )
        malloc_helper(sizeof(struct cache_set_associative) * sizes_length,
                      "cache_state_init() : cache_set_associative");
    
    cstate->dm_count = sizes_length;
    cstate->fa_count = sizes_length;
    cstate->sa_count = sizes_length;

    for (int i = 0; i < sizes_length; i++) {
        cache_direct_mapped_init(&dm_array[i], sizes[i]);
        cache_fully_associative_init(&fa_array[i], sizes[i]);
        cache_set_associative_init(&sa_array[i], sizes[i]);
    }

    cstate->dm_array = dm_array;
    cstate->fa_array = fa_array;
    cstate->sa_array = sa_array;
}

/* Cache deallocation */
void cache_state_free(struct cache_state *cstate) {
    int i;

    for (i = 0; i < cstate->dm_count; i++) {
        cache_direct_mapped_free(&cstate->dm_array[i]);
    }

    for (i = 0; i < cstate->fa_count; i++) {
        cache_fully_associative_free(&cstate->fa_array[i]);
    }

    for (i = 0; i < cstate->sa_count; i++) {
        cache_set_associative_free(&cstate->sa_array[i]);
    }
   
    free(cstate->dm_array);
    free(cstate->fa_array);
    free(cstate->sa_array);
}
    
/* Cache State Statistics */
void cache_state_stats(struct cache_state *cstate) {
    int i;
    printf("****************************Cache Stats*******************************\n"); 
    printf("---------------------------Direct mapped------------------------------\n");
    for(i = 0; i < cstate->dm_count; i++){
        cache_direct_mapped_stats(&cstate->dm_array[i]);
    }
    
    printf("---------------------------Fully Associative--------------------------\n");

    for(i = 0; i < cstate->fa_count; i++){
        cache_fully_associative_stats(&cstate->fa_array[i]);
    }
    printf("---------------------------Set Associative----------------------------\n");
    
    for(i = 0; i < cstate->sa_count; i++){
        cache_set_associative_stats(&cstate->sa_array[i]);
    }
    printf("----------------------------------------------------------------------\n");

}

/* Cache Lookup */
unsigned int cache_lookup(struct cache_state *cstate, unsigned int addr){
    unsigned int data;
    int i;

    for(i = 0; i < cstate->dm_count; i++){
        data = cache_lookup_direct_mapped(&cstate->dm_array[i], addr);
    }
    
    for(i = 0; i < cstate->fa_count; i++){
        data = cache_lookup_fully_associative(&cstate->fa_array[i], addr);
    }
    
    for(i = 0; i < cstate->sa_count; i++){
        data = cache_lookup_set_associative(&cstate->sa_array[i], addr);
    }

    return data;
}
