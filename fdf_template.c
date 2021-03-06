#include "fdf_template.h"
#include "fdf_types_full.h"
#include "fdf_enums.h"
#include "fdf_data_types.h"

#include <stdlib.h>

fdf_template *fdf_template_init()
{
	fdf_template *templ = malloc(sizeof(fdf_template));
	templ->time_type = FDF_DATA_INT32;
	templ->data_type = FDF_DATA_DOUBLE;
	templ->dimension = 0;
	templ->fixed_grid_size = 1;

	return templ;
}


void fdf_template_destroy( fdf_template *templ )
{
	free(templ);
}


int fdf_template_set_time_type( fdf_template *templ, unsigned int type )
{
	if( fdf_verify_data_type(type) ){
		templ->time_type = type;
		return FDF_SUCCESS;
	}else{
		return FDF_UNKNOWN_DATA_TYPE;
	}
}


unsigned int fdf_template_get_time_type( const fdf_template *templ )
{
	return templ->time_type;
}


int fdf_template_set_data_type( fdf_template *templ, unsigned int type )
{
	if( fdf_verify_data_type(type) ){
		templ->data_type = type;
		return FDF_SUCCESS;
	}else{
		return FDF_UNKNOWN_DATA_TYPE;
	}
}


unsigned int fdf_template_get_data_type( const fdf_template *templ )
{
	return templ->data_type;
}




void fdf_template_set_fixed_grid( fdf_template *templ, unsigned int value )
{
	templ->fixed_grid_size = value;
}

int fdf_template_is_fixed_grid( fdf_template *templ )
{
	return templ->fixed_grid_size;
}



int fdf_template_set_dimension( fdf_template *templ, int dim )
{
	if( dim < 1 || dim > 2 ){
		fprintf( stderr, "Dimension has to be 1 or 2!!\n" );
		return FDF_INCORRECT_DIMENSION;
	}
	templ->dimension = dim;
	return FDF_SUCCESS;
}


int fdf_template_get_dimension( const fdf_template *templ )
{
	return templ->dimension;
}
