/**
   \file main.c

   Contains a simple example for using FDF files.
*/

#include "fdf.h"

#include <assert.h>
#include <stdio.h>

int main( int argc, char **argv )
{
	fdf_file *bf = fdf_open( "test.fdf", "wb" );

	double data[6]  = {-1,-1,-1,-1,-1,-1};
	double data2[6] = {0,0,0,0,0,0};

	data[0] = -2.0;
	data[1] = 2.0;
	data[2] = 4.0;
	data[3] = 8.0;
	data[4] = 16.0;
	data[5] = 32.0;

	int time = 3;
	bm_1d_template templ;
	bm_1d_template templ2;

	fdf_1d_template_init( &templ );
	fdf_1d_template_set_size( &templ, 6 );
	fdf_1d_template_set_data_type( &templ, FDF_DATA_DOUBLE );
	fdf_1d_template_set_time_type( &templ, FDF_DATA_INT32 );

	fdf_write_1d( bf, &templ, &time, data );
	fdf_close( bf );

	fdf_file *bf2 = fdf_open( "test.fdf", "rb" );

	fdf_read_1d( bf2, &templ2, &time, data2 );
	for( size_t i = 0; i < 6; ++i ){
		assert( data[i] == data2[i] &&
		        "Incorrect read from written file" );
	}

	fdf_close( bf );

	return 0;
}
