#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "cut.h"
#include "cutlist.h"

int main(int argc, char *argv[]) {

  int rod_length;

  if (argc != 2 || !strcmp(argv[1], "-h"))
    return 1;

  ProviderFunction cut_fits_provider = cut_fits;
  Cache *cache = NULL;
  cache = load_cache_module("./least_recently_used.so");
  cut_fits_provider = cache->set_provider_func(cut_fits_provider);

  Cut *input_cuts = read_cuts_into_list(argv[1]);

  while ((rod_length = read_rod_length()) != ROD_LENGTH_ERROR) {
    CutList optimal_cutlist = find_optimal_cuts(input_cuts, rod_length);
    print_list(optimal_cutlist);
    free(optimal_cutlist.cuts);
  }

  free(input_cuts);

  return 0;
}
