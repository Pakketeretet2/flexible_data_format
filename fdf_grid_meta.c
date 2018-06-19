#include "fdf_grid_meta.h"
#include "fdf_types_full.h"
#include "fdf_data_types.h"
#include "fdf_enums.h"

#include <stdlib.h>

fdf_grid_meta *fdf_grid_meta_init()
{
	fdf_grid_meta *p = malloc(sizeof(fdf_grid_meta));
	if( p != NULL ){
		p->type = FDF_DATA_DOUBLE;
		p->size = 0;
	}
	return p;
}


void fdf_grid_meta_destroy( fdf_grid_meta *p )
{
	free(p);
}


unsigned int fdf_grid_meta_get_type( const fdf_grid_meta *grid_spec )
{
	return grid_spec->type;
}


int fdf_grid_meta_set_type( fdf_grid_meta *grid_spec, unsigned int type )
{
	if( fdf_verify_data_type( type ) ){
		grid_spec->type = type;
	}
	return FDF_SUCCESS;
}


int fdf_grid_meta_get_size( const fdf_grid_meta *grid_spec )
{
	return grid_spec->size;
}


int fdf_grid_meta_set_size( fdf_grid_meta *grid_spec, int size )
{
	if( size <= 0 ){
		return FDF_INVALID_GRID_SIZE;
	}
	grid_spec->size = size;
	return FDF_SUCCESS;
}
