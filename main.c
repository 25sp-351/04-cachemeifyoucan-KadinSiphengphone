#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "cut.h"
#include "cutlist.h"

#include <dlfcn.h>
int main(int argc, char *argv[]) {

  int rod_length;

  if (argc != 2 || !strcmp(argv[1], "-h"))
    return 1;

  int cache_initialized = 0;

  ProviderFunction choose_best_cuts_provider = NULL;

  Cache *cache = NULL;

  // choose_best_cuts_provider = choose_best_cuts;
  choose_best_cuts_provider = choose_best_cuts_to_key;

  cache = load_cache_module("./LRU.so");

  choose_best_cuts_provider =
      cache->set_provider_func(choose_best_cuts_provider);

  Cut *input_cuts = read_cuts_into_list(argv[1]);
  printf("Input rod length:\n");
  while ((rod_length = read_rod_length()) != ROD_LENGTH_ERROR) {
    // CutList optimal_cutlist = find_optimal_cuts(input_cuts, rod_length);
    CutList optimal_cutlist =
        choose_best_cuts_provider((KeyType){input_cuts, rod_length});
    print_list(optimal_cutlist);
    // free(optimal_cutlist.cuts);

    if (cache && cache->get_statistics) {
      CacheStat *stats = cache->get_statistics();
      for (int xx = 0; stats[xx].type != END_OF_STATS; xx++) {
        printf("%s: %d\n", CacheStatNames[stats[xx].type], stats[xx].value);
      }
    }
  }

  free(input_cuts);

  return 0;
}
