#include "fdf.h"
#include "fdf_types_full.h"


#include <stdlib.h>

/*
  This function reads the file and stores the data in storage
  that it allocates itself
*/
int fdf_read_fixed_grid_meta( fdf_file *f, const fdf_template *templ,
			      int *dimension, void ***grids, int **grid_sizes,
			      unsigned int **grid_types )
{
	if( !templ->fixed_grid_size ){
		return FDF_FILE_NOT_FIXED_GRID;
	}

	fdf_read_dimension( f, dimension );

	// Read all the grids:
	*grids      = malloc( *dimension*sizeof(void*) );
	*grid_sizes = malloc( *dimension*sizeof(int) );
	*grid_types = malloc( *dimension*sizeof(unsigned int) );


	for( int i = 0; i < *dimension; ++i ){
		fdf_read_grid_meta( f, grid_sizes[i], (*grid_types)[i] );
		int size_grid_type = fdf_data_type_to_rw_size( (*grid_types)[i] );
		grids[i] = malloc( size_grid_type*(*grid_sizes)[i] );
	}



	return FDF_SUCCESS;
}
