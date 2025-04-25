#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

#define CACHE_SIZE 3
#define NO_VALUE_PRESENT                                                       \
  (ValueType) { NULL, 0, 0 }
#define NO_KEY_PRESENT                                                         \
  (KeyType) { NULL, -1 }
#define NO_TIME_PRESENT -1

int time_info[CACHE_SIZE];

int cache_requests = 0;
int cache_hits = 0;
int cache_misses = 0;
int cache_evictions = 0;

int current_time = 0;
KeyType keys[CACHE_SIZE];
ValueType values[CACHE_SIZE];

ProviderFunction _downstream = NULL;

KeyType new_key(Cut *input_cuts, int rod_length) {
  KeyType key;
  key.input_cuts = copy_cuts(input_cuts);
  key.rod_length = rod_length;
  return key;
}

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
}

bool equals(KeyType first, KeyType second) {

  return cuts_equal(first.input_cuts, second.input_cuts) &&
         first.rod_length == second.rod_length;
}

ValueType _caching_provider(KeyType key) {

  fprintf(stderr, __FILE__ " caching_provider(" KEY_FMT ")\n", key.input_cuts,
          key.rod_length);
  cache_requests++;

  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    if (equals(keys[xx], key)) {
      cache_hits++;
      time_info[xx] = current_time++;
      return values[xx];
    }
  }

  cache_misses++;

  ValueType result = (*_downstream)(key);
  int least_recent_time = time_info[0];
  int least_recent_index = 0;

  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    if (time_info[xx] == NO_TIME_PRESENT) {
      keys[xx] = new_key(key.input_cuts, key.rod_length);
      values[xx] = result;
      time_info[xx] = current_time++;

      return result;
    }
    if (time_info[xx] < least_recent_time) {
      least_recent_time = time_info[xx];
      least_recent_index = xx;
    }
  }

  if (time_info[least_recent_index] != NO_TIME_PRESENT) {
    cache_evictions++;
    free(keys[least_recent_index].input_cuts);
    keys[least_recent_index] = new_key(key.input_cuts, key.rod_length);
    values[least_recent_index] = result;
    time_info[least_recent_index] = current_time++;
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
    time_info[xx] = NO_TIME_PRESENT;
  }

  fprintf(stderr, __FILE__ " initialize()\n");
}
void cleanup(void) {
  for (int xx = 0; xx < CACHE_SIZE; xx++) {
    if (keys[xx].input_cuts) {
      free(keys[xx].input_cuts);
    }
  }

  fprintf(stderr, __FILE__ " cleanup()\n");
}
