#include "fdf_file.h"
#include "fdf_data_types.h"


#include <stdint.h>
#include <stdlib.h>
#include <limits.h>


fdf_file *fdf_open( const char *fname, const char *mode )
{
	fdf_file *bm = malloc( sizeof( fdf_file ) );
	bm->f = fopen( fname, mode );
	return bm;
}


void fdf_close( fdf_file *bm )
{
	fclose( bm->f );
	free(bm);
	bm = NULL;
}


/**
   \brief Casts and writes the time stamp the correct way.
*/
void fdf_write_time_stamp( fdf_file *bm, const bm_1d_template *templ,
                           const void *tstamp )
{
	switch( templ->time_type ){
		case FDF_DATA_INT32:{
			const int32_t *typed_time = (const int32_t *) tstamp;
			fwrite( typed_time, sizeof( int32_t ), 1, bm->f );
			break;
		}
		case FDF_DATA_INT64:{
			const int64_t *typed_time = (const int64_t *) tstamp;
			fwrite( typed_time, sizeof( int64_t ), 1, bm->f );
			break;
		}

		case FDF_DATA_DOUBLE:{
			const double *typed_time = (const double *) tstamp;
			fwrite( typed_time, sizeof( double ), 1, bm->f );
			break;
		}
	}
}



/**
   \brief Reads the time stamp the correct way.
*/
void fdf_read_time_stamp( fdf_file *bm, const bm_1d_template *templ,
                          void *tstamp )
{
	unsigned int size_to_read = 0;
	switch( templ->time_type ){
		case FDF_DATA_INT32:
			size_to_read = sizeof(int32_t);
			break;

		case FDF_DATA_INT64:
			size_to_read = sizeof(int64_t);
			break;

		case FDF_DATA_DOUBLE:
			size_to_read = sizeof(double);
			break;
	}
	fread( tstamp, size_to_read, 1, bm->f );
	// return SUCCESS;
}


/**
   \brief Casts and writes the data the correct way.
*/
void fdf_write_data( fdf_file *bm, const bm_1d_template *templ,
                     const void *data )
{
	size_t size_to_write = 0;
	switch( templ->data_type ){
		case FDF_DATA_INT32:{
			size_to_write = sizeof(int32_t);
			break;
		}
		case FDF_DATA_INT64:{
			size_to_write = sizeof(int64_t);
			break;
		}

		case FDF_DATA_DOUBLE:{
			size_to_write = sizeof(double);
			break;
		}
	}
	unsigned int bytes = fwrite( data, size_to_write, templ->N, bm->f );
}



/**
   \brief Reads the time stamp the correct way.
*/
void fdf_read_data( fdf_file *bm, const bm_1d_template *templ,
                    void *data )
{
	size_t size_to_read = 0;
	switch( templ->data_type ){
		case FDF_DATA_INT32:
			size_to_read = sizeof(int32_t);
			break;

		case FDF_DATA_INT64:
			size_to_read = sizeof(int64_t);
			break;

		case FDF_DATA_DOUBLE:
			size_to_read = sizeof(double);
			break;
	}

	unsigned int bytes = fread( data, size_to_read, templ->N, bm->f );
	// return SUCCESS;
}



/**
   Make sure the internal format is consistent and mirrored
   between write and read. Format is:
   <dimension> (int)
   <corresponding template>
   <time stamp>
   <data>
*/
int fdf_write_1d( fdf_file *bm, const bm_1d_template *templ,
                  const void *tstamp, const void *data )
{
	int dim = 1;
	fwrite( &dim, sizeof(int), 1, bm->f );
	fwrite( templ, sizeof(bm_1d_template), 1, bm->f );
	fdf_write_time_stamp( bm, templ, tstamp );
	fdf_write_data( bm, templ, data );

	return SUCCESS;
}



int fdf_read_1d( fdf_file *bm, bm_1d_template *templ,
                 void *tstamp, void *data )
{
	int dim_check = 0;
	fread( &dim_check, sizeof(int), 1, bm->f );
	if( dim_check != 1 ){
		return READ_DIMENSION_MISMATCH;
	}
	fread( templ, sizeof(bm_1d_template), 1, bm->f );
	fdf_read_time_stamp( bm, templ, tstamp );
	fdf_read_data( bm, templ, data );

	return SUCCESS;
}


void fdf_1d_template_init( bm_1d_template *templ )
{
	templ->N = 0;
	templ->time_type = FDF_DATA_INT32;
	templ->data_type = FDF_DATA_DOUBLE;
}



void fdf_1d_template_set_size( bm_1d_template *templ, unsigned int size )
{
	templ->N = size;
}


void fdf_1d_template_set_time_type( bm_1d_template *templ, unsigned int type )
{

	templ->time_type = type;
}


void fdf_1d_template_set_data_type( bm_1d_template *templ, unsigned int type )
{
	templ->data_type = type;
}


unsigned int fdf_1d_template_get_size( const bm_1d_template *templ )
{
	return templ->N;
}


unsigned int fdf_1d_template_get_time_type( const bm_1d_template *templ )
{
	return templ->time_type;
}


unsigned int fdf_1d_template_get_data_type( const bm_1d_template *templ )
{
	return templ->data_type;
}
