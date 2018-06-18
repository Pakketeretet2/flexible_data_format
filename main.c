/**
   \file main.c

   Contains a simple example for using FDF files.
*/

#include "fdf.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/**
   \brief This function showcases the recommended way to write
   1D data on a fixed grid to a file and how to read it back.
*/
int test_1d_read_write()
{
	double x[6] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
	double data[6] = { 2.0, 3.0, 4.0, 8.0, 16.0, -2.0 };
	int status = 0;

	fdf_template *templ = fdf_template_init();
	fdf_template_set_time_type( templ, FDF_DATA_INT32 );
	fdf_template_set_data_type( templ, FDF_DATA_DOUBLE );
	fdf_template_set_fixed_grid( templ, 1 );

	fdf_file *f1 = fdf_open( "test.fdf", FDF_WRITE_ONLY, &status );
	if( status != FDF_SUCCESS ){
		return -1;
	}

	unsigned int bytes = 0;
	int dimension = 1;
	bytes += fdf_write_template( f1, templ );
	bytes += fdf_write_dimension( f1, dimension );
	bytes += fdf_write_grid( f1, FDF_DATA_DOUBLE, 6, x );

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
	fprintf( stderr, "Wrote %u bytes to file \"test.fdf\".\n", bytes );

	fprintf( stderr, "\nAttempting to read out file \"test.fdf\".\n" );
	fdf_file *f2 = fdf_open( "test.fdf", FDF_READ_ONLY, &status );
	if( status != FDF_SUCCESS ){
		return -1;
	}
	fdf_template *templ2 = fdf_template_init();
	fdf_read_template( f2, templ2 );
	fprintf( stderr, "Read out template: %u, %u, %d\n",
		 fdf_template_get_data_type( templ2 ),
		 fdf_template_get_time_type( templ2 ),
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

	int dimension2 = 0;
	int code = 0;
	fprintf( stderr, "Reading dimension...\n" );
	fdf_read_dimension( f2, &dimension2 );
	if( dimension2 != dimension ){
		code = -1;
		goto clean_templ2;
	}

	fprintf( stderr, "Dimension is %d, reading grid.\n", dimension2 );
	// Read grid:
	int grid_size_x = 0;
	unsigned int grid_type = 0;
	status = fdf_read_grid_meta( f2, &grid_size_x, &grid_type );


	fprintf( stderr, "Grid size is %d, grid type is %u\n",
		 grid_size_x, grid_type );
	if( status ){
		code = -1;
		goto clean_templ2;
	}
	if( status ){
		code = -1;
		goto clean_templ2;
	}
	if( grid_type != FDF_DATA_DOUBLE ){
		code = -1;
		goto clean_templ2;
	}

	unsigned int size_to_alloc = sizeof(double) * grid_size_x;
	double *x2 = malloc( size_to_alloc );
	double *data2 = malloc( sizeof(double)*grid_size_x );
	status = fdf_read_grid( f2, grid_type, grid_size_x, x2 );
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

	int dim = 0;
	void **grids = NULL;
	int *grid_sizes = NULL;
	unsigned int *grid_types = NULL;

	status = fdf_read_fixed_grid_meta( f, templ, &dim, &grids, &grid_sizes, &grid_types );
	fprintf( stderr, "File has %d dimensions. Here is each grid size and type:\n", dim );
	for( int i = 0; i < dim; ++i ){
		fprintf( stderr, "  %d %u\n", grid_sizes[i], grid_types[i] );
	}

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
