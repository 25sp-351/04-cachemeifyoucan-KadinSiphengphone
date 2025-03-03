#ifndef CUT_H
#define CUT_H

#define MAX_LIST_SIZE 50
#define END_OF_LIST -1
#define ROD_LENGTH_ERROR -999

typedef struct {
  int length;
  int value;
  int amount_of_cuts;
} Cut;

Cut *read_cuts_into_list();
int size_of_cuts(Cut *cuts);
Cut *copy_cuts(Cut *cuts);
Cut *create_empty_cuts(Cut *cuts);
Cut *add_cut(Cut *cuts, int length);
int read_rod_length();
#endif