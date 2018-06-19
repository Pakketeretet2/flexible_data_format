/**
   \file main.c

   Contains a simple example for using FDF files.
*/

#include "fdf.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>





/**
   \brief This function showcases the recommended way to write
   1D data on a fixed grid to a file and how to read it back.
*/
int test_1d_read_write()
{
	double x[6] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
	double data[6] = { 2.0, 3.0, 4.0, 8.0, 16.0, -2.0 };
	int dimension = 1;
	int status = 0;

	fdf_template *templ = fdf_template_init();
	fdf_template_set_time_type( templ, FDF_DATA_INT32 );
	fdf_template_set_data_type( templ, FDF_DATA_DOUBLE );
	fdf_template_set_dimension( templ, dimension );
	fdf_template_set_fixed_grid( templ, 1 );


	fdf_file *f1 = fdf_open( "test.fdf", FDF_WRITE_ONLY, &status );
	if( status != FDF_SUCCESS ){
		return -1;
	}

	unsigned int bytes = 0;
	bytes += fdf_write_template( f1, templ );
	fdf_grid_meta *grid_spec = fdf_grid_meta_init();
	fdf_grid_meta_set_type( grid_spec, FDF_DATA_DOUBLE );
	fdf_grid_meta_set_size( grid_spec, 6 );

	bytes += fdf_write_grid_meta( f1, grid_spec );
	bytes += fdf_write_grid_data( f1, grid_spec, x );


	int t = 23;
	for( int j = 0; j < 10; ++j ){
		bytes += fdf_write_time( f1, templ, &t );
		bytes += fdf_write_data_1d( f1, templ, 6, data );
		for( int i = 0; i < 6; ++i ){
			data[i] += 1.0;
		}
		t++;
	}
	fdf_close( f1 );
	fdf_template_destroy( templ );
	fdf_grid_meta_destroy( grid_spec );

	fprintf( stderr, "Wrote %u bytes to file \"test.fdf\".\n", bytes );

	fprintf( stderr, "\nAttempting to read out file \"test.fdf\".\n" );
	fdf_file *f2 = fdf_open( "test.fdf", FDF_READ_ONLY, &status );
	if( status != FDF_SUCCESS ){
		return -1;
	}
	fprintf( stderr, "Succesfully opened file \"test.fdf\" for reading.\n" );
	fdf_template *templ2 = fdf_template_init();
	fdf_read_template( f2, templ2 );
	fprintf( stderr, "Read out template: { %u, %u, %d, %d }\n",
		 fdf_template_get_data_type( templ2 ),
		 fdf_template_get_time_type( templ2 ),
	         fdf_template_get_dimension( templ2 ),
		 fdf_template_is_fixed_grid( templ2 ) );

	if( fdf_template_get_time_type(templ2) != FDF_DATA_INT32 ){
		return -1;
	}
	if( fdf_template_is_fixed_grid(templ2) != 1 ){
		return -1;
	}
	if( fdf_template_get_data_type(templ2) != FDF_DATA_DOUBLE ){
		return -1;
	}
	if( fdf_template_get_dimension(templ2) != 1 ){
		return -1;
	}

	int dimension2 = fdf_template_get_dimension( templ2 );
	int code = 0;
	if( dimension2 != dimension ){
		code = -1;
		goto clean_templ2;
	}

	fprintf( stderr, "Dimension is %d, reading grid.\n", dimension2 );
	// Read grid:
	fdf_grid_meta *grid_spec2 = fdf_grid_meta_init();
	status = fdf_read_grid_meta( f2, grid_spec2 );


	fprintf( stderr, "Grid size is %d, grid type is %u\n",
	         fdf_grid_meta_get_size(grid_spec2),
	         fdf_grid_meta_get_type(grid_spec2) );
	if( status ){
		code = -1;
		goto free_grid_spec2;
	}
	if( status ){
		code = -1;
		goto free_grid_spec2;
	}
	if( fdf_grid_meta_get_type(grid_spec2) != FDF_DATA_DOUBLE ){
		code = -1;
		goto free_grid_spec2;
	}

	int grid_size_x = fdf_grid_meta_get_size( grid_spec2 );
	unsigned int size_to_alloc = sizeof(double) * grid_size_x;
	double *x2 = malloc( size_to_alloc );
	double *data2 = malloc( sizeof(double)*grid_size_x );

	status = fdf_read_grid_data( f2, grid_spec2, x2 );
	if( status ){
		code = -1;
		goto free_x2;
	}
	fprintf( stderr, "Read grid succesfully.\n" );

	int t2 = 0;
	t = 23;
	for( int j = 0; j < 10; ++j ){
		fdf_read_time( f2, templ2, &t2 );
		if( t2 != t ){
			code = -1;
			goto free_x2;
		}
		status = fdf_read_data_1d( f2, templ2, grid_size_x, data2 );
		if( t2 == 32 ){
			for( int i = 0; i < grid_size_x; ++i ){
				if( data[i] != data2[i] + 1 ){
					fprintf( stderr, "Error, %g != %g\n", data[i], data2[i] );
				}
			}
		}
		if( status ){
			code = -2;
			goto free_x2;
		}
		t++;
	}

free_x2:
	free(data2);
	free(x2);
free_grid_spec2:
	fdf_grid_meta_destroy( grid_spec2 );
clean_templ2:
	fdf_template_destroy( templ2 );
	fdf_close( f2 );

	return code;
}



int test_high_level()
{
	int status = 0;
	fdf_file *f = fdf_open( "test.fdf", FDF_READ_ONLY, &status );
	fdf_template *templ = fdf_template_init();

	fdf_read_template( f, templ );

	void **grids = NULL;
	fdf_grid_meta **grid_specs = NULL;
	status = fdf_init_grid_meta( templ, &grid_specs );
	if( status != FDF_SUCCESS ){
		fprintf( stderr, "Error initing grid meta!\n" );
	}
	// At this point grid_specs is an array with one or two pointers
	// to fdf_grid_meta in it.

	int dim = fdf_template_get_dimension( templ );

	status = fdf_read_grid_meta( f, grid_specs[0] );
	if( status != FDF_SUCCESS ){
		fprintf( stderr, "Error reading grid meta!\n" );
	}else{
		fprintf( stderr, "grid_spec[0]: %u %d\n",
		         fdf_grid_meta_get_type(grid_specs[0]),
		         fdf_grid_meta_get_size(grid_specs[0]));
	}

	if( dim == 2 ){
		status = fdf_read_grid_meta( f, grid_specs[1] );
		if( status != FDF_SUCCESS ){
			fprintf( stderr, "Error reading grid meta!\n" );
		}else{
			fprintf( stderr, "grid_spec[1]: %u %d\n",
			         fdf_grid_meta_get_type(grid_specs[1]),
			         fdf_grid_meta_get_size(grid_specs[1]));
		}
	}


	void *raw_data;
	int32_t time = 0;

	status = fdf_init_grid( templ, grid_specs, &grids );
	if( status != FDF_SUCCESS ){
		fprintf( stderr, "Error initting grid!\n" );
	}
	fprintf( stderr, "Succesfully allocated grids.\n" );

	status |= fdf_init_data( templ, grid_specs[0], &raw_data );
	if( status != FDF_SUCCESS ){
		fprintf( stderr, "Error initting data!\n" );
	}

	// Read the grid:
	status = fdf_read_grid_data( f, grid_specs[0], grids[0] );
	if( dim == 2 ){
		status = fdf_read_grid_data( f, grid_specs[1], grids[1] );
	}

	// double *data = (double*)data_ptr;
	double *grid = grids[0];
	double *data = raw_data;

	while( fdf_read_data_block( f, templ, grid_specs[0], &time, raw_data ) == FDF_SUCCESS ){
		for( int i = 0; i < fdf_grid_meta_get_size(grid_specs[0]); ++i ){
			fprintf( stderr, "t = %d:  x[%d]=%g, d[%d]=%g\n", time, i,
			         grid[i], i, data[i] );
		}
		fprintf( stderr, "\n" );
	}


	fdf_destroy_data( data );
	fdf_destroy_grid( templ, grids );
	for( int i = 0; i < fdf_template_get_dimension(templ); ++i ){
		fdf_grid_meta_destroy( grid_specs[i] );
	}
	fdf_template_destroy( templ );

	return 0;
}


int main( int argc, char **argv )
{
	if( test_1d_read_write() == 0 ){
		fprintf( stderr, "1d read/write works as expected.\n" );
	}else{
		fprintf( stderr, "1d read/write does not work as expected.\n" );
	}

	test_high_level();

	return 0;
}
