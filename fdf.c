#include "fdf.h"
#include "fdf_types_full.h"


#include <stdlib.h>

/*
  This function reads the file and stores the data in storage
  that it allocates itself
*/
int fdf_read_grids( fdf_file *f, const fdf_template *templ,
                    int *dimension, void ***grids, int **grid_sizes,
                    unsigned int **grid_types )
{
	fdf_read_dimension( f, dimension );

	// Read all the grids:
	*grids      = malloc( *dimension*sizeof(void*) );
	*grid_sizes = malloc( *dimension*sizeof(int) );
	*grid_types = malloc( *dimension*sizeof(unsigned int) );

	for( int i = 0; i < *dimension; ++i ){
		fdf_read_grid_meta( f, grid_sizes[i], grid_types[i] );
		int size_grid_type = fdf_data_type_to_rw_size( (*grid_types)[i] );
		(*grids)[i] = malloc( size_grid_type*(*grid_sizes)[i] );
	}

	for( int i = 0; i < *dimension; ++i ){
		fdf_read_grid( f, (*grid_sizes)[i],
		               (*grid_types)[i], (*grids)[i] );
	}

	return FDF_SUCCESS;
}




unsigned int fdf_write_grids( fdf_file *f, const fdf_template *templ,
                              int dimension, void **grids, int *grid_sizes,
                              unsigned int *grid_types )
{

	unsigned int bytes = 0;

	for( int i = 0; i < dimension; ++i ){
		bytes += fdf_write_grid_meta( f, grid_sizes[i], grid_types[i] );
	}
	for( int i = 0; i < dimension; ++i ){
		bytes += fdf_write_grid_data( f, grid_sizes[i],
		                              grid_types[i], grids[i] );
	}

	return FDF_SUCCESS;
}




int fdf_destroy_grids( int dimension, void ***grids, int **grid_sizes,
                       unsigned int **grid_types )
{
	for( int i = 0; i < dimension; ++i ){
		free((*grids)[i]);
	}
	free(*grids);
	free(*grid_sizes);
	free(*grid_types);

	return FDF_SUCCESS;
}



int fdf_read_data( fdf_file *f, const fdf_template *templ,
                   int dimension, const int *grid_sizes,
                   void *time, void *data )
{
	fdf_read_time( f, templ, time );
	if( dimension == 1 ){
		return fdf_read_data_1d( f, templ, grid_sizes[0], data );
	}else if( dimension == 2 ){
		return fdf_read_data_2d( f, templ, grid_sizes[0],
		                         grid_sizes[1], data );
	}else{
		return 0;
	}
}
