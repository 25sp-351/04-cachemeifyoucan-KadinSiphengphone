#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"
#include "cutlist.h"

#define CACHE_SIZE 10
#define NO_VALUE_PRESENT -1
#define NO_KEY_PRESENT                                                         \
  (KeyType) { -1, -1 }

int cache_requests = 0;
int cache_hits = 0;
int cache_misses = 0;
int cache_evictions = 0;

int times[CACHE_SIZE];
int current_time = 0;
KeyType keys[CACHE_SIZE];
ValueType values[CACHE_SIZE];

ProviderFunction _downstream = NULL;

CacheStat *statistics(void) {
  fprintf(stderr, __FILE__ "statistics()\n");
  CacheStat *current_stats = malloc(5 * sizeof(CacheStat));
  current_stats[0] = (CacheStat){Cache_requests, cache_requests};
  current_stats[1] = (CacheStat){Cache_hits, cache_hits};
  current_stats[2] = (CacheStat){Cache_misses, cache_misses};
  current_stats[3] = (CacheStat){Cache_evictions, cache_evictions};
  current_stats[4] = (CacheStat){END_OF_STATS, 0};

  return current_stats;
}

void reset_statistics(void) {
  fprintf(stderr, __FILE__ " reset_statistics()\n");
  cache_requests = 0;
  cache_hits = 0;
  cache_misses = 0;
  cache_evictions = 0;
  current_time = 0;
}

bool equals(KeyType first, KeyType second) {
  return first.cut_length == second.cut_length &&
         first.rod_length == second.rod_length;
}

ValueType _caching_provider(KeyType key) {

  fprintf(stderr, __FILE__ " caching_provider(" KEY_FMT ")\n", key.cut_length,
          key.rod_length);
  cache_requests++;

  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    if (equals(keys[xx], key)) {
      cache_hits++;
      times[xx] = current_time++;
      return values[xx];
    }
  }

  cache_misses++;

  ValueType result = (*_downstream)(key);
  int least_recent_time = times[0];
  int least_recent_index = 0;

  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    if (times[xx] == NO_VALUE_PRESENT) {
      keys[xx] = key;
      values[xx] = result;
      times[xx] = current_time++;
      return result;
    }
    if (times[xx] < least_recent_time) {
      least_recent_time = times[xx];
      least_recent_index = xx;
    }
  }

  if (times[least_recent_index] != NO_VALUE_PRESENT) {
    cache_evictions++;
    keys[least_recent_index] = key;
    values[least_recent_index] = result;
    times[least_recent_index] = current_time++;
  }

  return result;
}

ProviderFunction set_provider(ProviderFunction downstream) {
  fprintf(stderr, __FILE__ " set_provider()\n");
  _downstream = downstream;
  return _caching_provider;
}

void initialize(void) {
  cache_requests = 0;
  cache_hits = 0;
  cache_misses = 0;
  cache_evictions = 0;
  current_time = 0;
  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    keys[xx] = NO_KEY_PRESENT;
    values[xx] = NO_VALUE_PRESENT;
    times[xx] = NO_VALUE_PRESENT;
  }
  fprintf(stderr, __FILE__ " initialize()\n");
}
void cleanup(void) {
  // free yo junk

  fprintf(stderr, __FILE__ " cleanup()\n");
}

ValueType
