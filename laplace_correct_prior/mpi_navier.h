#include <mpi.h>

extern int my_rank;
extern int prev_y;
extern int next_y;
extern int next_x;
extern int prev_x;

extern int imax_full;
extern int jmax_full;

extern int gbl_i_begin;
extern int gbl_j_begin;

void mpi_setup(int *imax, int *jmax);
void exchange_halo(int imax, int jmax, double *arr);
