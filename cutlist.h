#ifndef CUTLIST_H
#define CUTLIST_H

#include "cache.h"
#include "cut.h"
typedef struct {
  Cut *cuts;
  int remainder;
  int total_value;
} CutList;

CutList new_cutlist(Cut *cuts, int rod_length, int total_value);
int cut_fits(int cut_length, int rod_length);
CutList find_optimal_cuts(Cut *cuts, int rod_length);
void print_list(CutList cutlist);
void free_cutlist(CutList *list);

#endif