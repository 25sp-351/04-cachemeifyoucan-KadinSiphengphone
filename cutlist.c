#include "cutlist.h"

#include <stdio.h>
#include <stdlib.h>

CutList new_cutlist(Cut *cuts, int rod_length, int total_value) {
  CutList list;
  list.cuts = cuts;
  list.remainder = rod_length;
  list.total_value = total_value;
  return list;
}

CutList find_optimal_cuts(Cut *cuts, int rod_length) {

  if (rod_length <= 0) {
    Cut *empty_cuts = create_empty_cuts(cuts);
    return new_cutlist(empty_cuts, rod_length, 0);
  }

  int max_value = 0;

  CutList best_cutlist = new_cutlist(NULL, rod_length, 0);

  for (int xx = 0; xx < size_of_cuts(cuts); xx++) {
    if (cuts[xx].length > rod_length) {
      continue;
    }

    int remainder = rod_length - cuts[xx].length;
    CutList possible_cutlist = find_optimal_cuts(cuts, remainder);

    int possible_value = possible_cutlist.total_value + cuts[xx].value;
    if (possible_value > max_value) {
      max_value = possible_value;

      if (best_cutlist.cuts) {
        free(best_cutlist.cuts);
      }

      best_cutlist = possible_cutlist;
      best_cutlist.cuts = add_cut(best_cutlist.cuts, cuts[xx].length);

      best_cutlist.total_value = max_value;

    } else {
      if (possible_cutlist.cuts) {
        free(possible_cutlist.cuts);
      }
    }
  }

  if (!best_cutlist.cuts) {
    Cut *empty_cuts = create_empty_cuts(cuts);
    return new_cutlist(empty_cuts, rod_length, 0);
  }

  int used_length = 0;
  for (int xx = 0; xx < size_of_cuts(best_cutlist.cuts); xx++) {
    used_length +=
        best_cutlist.cuts[xx].length * best_cutlist.cuts[xx].amount_of_cuts;
  }
  best_cutlist.remainder = rod_length - used_length;

  return best_cutlist;
}

CutList choose_best_cuts(Cut *cuts, int rod_length) {
  return find_optimal_cuts(cuts, rod_length);
}

void print_list(CutList cutlist) {
  if (!cutlist.cuts) {
    printf("No cutlist found.\n");
    return;
  }

  for (int xx = 0; xx < size_of_cuts(cutlist.cuts); xx++) {
    if (cutlist.cuts[xx].amount_of_cuts > 0) {
      int cut_total_value =
          cutlist.cuts[xx].value * cutlist.cuts[xx].amount_of_cuts;
      printf("%d @ %d = %d\n", cutlist.cuts[xx].amount_of_cuts,
             cutlist.cuts[xx].length, cut_total_value);
    }
  }

  printf("Remainder: %d\n", cutlist.remainder);
  printf("Value: %d\n", cutlist.total_value);
}

void free_cutlist(CutList *list) {
  if (list != NULL) {
    if (list->cuts != NULL) {
      free(list->cuts);
    }
    free(list);
  }
}
