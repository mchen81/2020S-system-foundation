/* cache.h - Public data structures and functions from cache.c */
#ifndef CACHE_H
#define CACHE_H

/* Cache simulation structs */

struct cache_entry {
    unsigned int valid;
    unsigned int tag;
    unsigned int data;
    /* The time_stamp can be used in fully associative and set associative
     * caches to approximate LRU.
     */
    unsigned int time_stamp;
};

struct cache_direct_mapped {
    struct cache_entry *cache_array;
    int size;
    int refs;
    int hits;
    int misses;
};

struct cache_fully_associative {
    struct cache_entry *cache_array;
    int size;
    int refs;
    int hits;
    int misses;
};

struct cache_set_associative {
    struct cache_entry *cache_array;
    int size;
    int refs;
    int hits;
    int misses;
};

struct cache_state {
    struct cache_direct_mapped *dm_array;
    int dm_count;

    struct cache_fully_associative *fa_array;
    int fa_count;
    
    struct cache_set_associative *sa_array;
    int sa_count;
};

void cache_state_init(struct cache_state *cstate);
void cache_state_free(struct cache_state *cstate);
void cache_state_stats(struct cache_state *cstate);
unsigned int cache_lookup(struct cache_state *cstate, unsigned int addr);
    
#endif /* CACHE_H */
