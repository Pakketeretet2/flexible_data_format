#include "fdf.h"
#include "fdf_types_full.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>


/*
  This function reads the file and stores the data in storage
  that it allocates itself
*/
int fdf_init_grid_meta( const fdf_template *templ,
                        fdf_grid_meta ***grid_specs )
{
	/* This function can abuse the memory layout of grid_specs.
	   It should simply be an array with one or two pointers to grid_specs.
	   Make the memory block contiguous.
	 */
	int dim = templ->dimension;
	*grid_specs = malloc( dim*sizeof(fdf_grid_meta*) );
	fdf_grid_meta *grid_specs_ = malloc( dim*sizeof(fdf_grid_meta) );
	if( !(*grid_specs) || !grid_specs_ ){
		return FDF_MALLOC_FAILED;
	}
	(*grid_specs)[0] = grid_specs_;
	if( dim == 2 ){
		(*grid_specs)[1] = grid_specs_ + 1;
	}

	return FDF_SUCCESS;
}


int fdf_destroy_grid_meta( fdf_grid_meta *grid_specs )
{
	free( grid_specs );
	return FDF_SUCCESS;
}




int fdf_init_grid( const fdf_template *templ,
                   fdf_grid_meta **grid_specs,
                   void ***grids )
{
	*grids = malloc( sizeof(void**) );
	if( !(*grids) ){
		return FDF_MALLOC_FAILED;
	}

	for( int i = 0; i < templ->dimension; ++i ){
		unsigned int type = grid_specs[i]->type;
		int size = grid_specs[i]->size;
		size_t alloc_size = fdf_data_type_to_ptr_size( type );
		(*grids)[i] = malloc( alloc_size*size );
		if( !(*grids)[i] ){
			return FDF_MALLOC_FAILED;
		}
	}
	return FDF_SUCCESS;
}


int fdf_init_data( const fdf_template *templ,
                   const fdf_grid_meta *grid_specs, void **data )
{
	size_t n_elements = grid_specs[0].size;
	if( templ->dimension == 2 ) n_elements *= grid_specs[1].size;

	size_t alloc_size = fdf_data_type_to_ptr_size( templ->data_type );
	*data = malloc( alloc_size*n_elements );
	if( !(*data) ){
		return FDF_MALLOC_FAILED;
	}

	/* After malloc-ing data it is the same as just calling read.
	   That is up to the user to do. */
	return FDF_SUCCESS;
}



int fdf_destroy_grid( const fdf_template *templ, void **grids )
{
	for( int i = 0; i <templ->dimension; ++i ){
		free(grids[i]);
	}
	return FDF_SUCCESS;
}


int fdf_destroy_data( void *data )
{
	free(data);
	return FDF_SUCCESS;
}


int fdf_read_data_block( fdf_file *f, const fdf_template *templ,
                         const fdf_grid_meta *grid_specs, void *time,
                         void *data )
{
	int n_expect = grid_specs[0].size;
	if( templ->dimension == 2 ) n_expect *= grid_specs[1].size;

	int status = fdf_read_time( f, templ, time );
	if( status != FDF_SUCCESS ){
		return status;
	}
	/* fdf_read_data_raw already checks for right size. */
	fdf_read_data_raw( f, templ, n_expect, data );

	return FDF_SUCCESS;
}
