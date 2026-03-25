#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

int my_rank;
int nprocs;
int nprocs_y;
int nprocs_x;
int my_rank_x;
int my_rank_y;
int prev_y;
int next_y;
int next_x;
int prev_x;
int imax_full;
int jmax_full;
int gbl_i_begin;
int gbl_j_begin;
MPI_Comm cartcomm;
MPI_Datatype row, column;

void mpi_setup(int *imax, int *jmax) {
	MPI_Init(NULL, NULL);
	//Initialise: get #of processes and process id
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//Check for compatible number of processes
	int dims[2] = {0,0};
	MPI_Dims_create(nprocs, 2, dims);
	if (my_rank == 0) std::cout << "Running with " << dims[0] << "x" << dims[1] << "processes \n";
	int periods[2] = {0,0};
	int reorder = 0;
        MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm);
	
	//Figure out process X,Y coordinates
	int coords[2];
        MPI_Cart_coords(cartcomm, my_rank, 2, coords);
	my_rank_x = coords[0];
	my_rank_y = coords[1];
	
	//Figure out neighbours
	MPI_Cart_shift(cartcomm, 0, 1, &prev_x, &next_x);
	MPI_Cart_shift(cartcomm, 1, 1, &prev_y, &next_y);
	
	//Save original full sizes in x and y directions
	imax_full = *imax;
	jmax_full = *jmax;
	
	//Modify imax and jmax (pay attention to integer divisions's rounding issues!)
	*imax = imax_full/dims[0];
	*jmax = jmax_full/dims[1];
	if (next_x == MPI_PROC_NULL) *imax = (imax_full - (dims[0]-1)* *imax);
	if (next_y == MPI_PROC_NULL) *jmax = (jmax_full - (dims[1]-1)* *jmax);
	
	//Figure out beginning i and j index in terms of global indexing
	gbl_i_begin = (imax_full / dims[0]) * my_rank_x;
	gbl_j_begin = (jmax_full / dims[1]) * my_rank_y;
	

	//Let's set up MPI Datatypes
	MPI_Type_vector((*imax+2), 1, 1, MPI_DOUBLE, &row);
	MPI_Type_vector((*jmax+2), 1, (*imax+2), MPI_DOUBLE, &column);
	MPI_Type_commit(&row);
	MPI_Type_commit(&column);


	
}

void exchange_halo(int imax, int jmax, double *arr) {
	//Exchange halos: prev_y, next_y, prev_x, next_x
	MPI_Sendrecv(&arr[(jmax+0)*(imax+2)], 1, row, next_y, 0,
	             &arr[0]                , 1, row, prev_y, 0,
		     cartcomm, MPI_STATUS_IGNORE);
	MPI_Sendrecv(&arr[(1)*(imax+2)],      1, row, prev_y, 0,
	             &arr[(jmax+1)*(imax+2)], 1, row, next_y, 0,
		     cartcomm, MPI_STATUS_IGNORE);
	MPI_Sendrecv(&arr[(imax+0)],      1, column, next_x, 0,
	             &arr[0]                , 1, column, prev_x, 0,
		     cartcomm, MPI_STATUS_IGNORE);
	MPI_Sendrecv(&arr[(1)],           1, column, prev_x, 0,
	             &arr[(imax+1)]         , 1, column, next_x, 0,
		     cartcomm, MPI_STATUS_IGNORE);
}








