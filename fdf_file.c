#include "fdf_file.h"
#include "fdf_data_types.h"
#include "fdf_data_1d.h"

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


unsigned int data_type_to_rw_size( unsigned int data_type )
{
	switch( data_type ){
		case FDF_DATA_INT32:
			return sizeof(int32_t);
		case FDF_DATA_INT64:
			return sizeof(int64_t);
		case FDF_DATA_DOUBLE:
			return sizeof(double);
		default:
			// Error?
			// assert( false && "Unknown data type!" );
			return 0;
	}
}


/**
   \brief Casts and writes the time stamp the correct way.
*/
unsigned int fdf_write_time_stamp( fdf_file *bm, const bm_1d_template *templ,
				   const void *tstamp )
{
	unsigned int write_size = data_type_to_rw_size(templ->time_type);
	unsigned int bytes = fwrite( tstamp, write_size, 1, bm->f );
	return bytes;
}



/**
   \brief Reads the time stamp the correct way.
*/
unsigned int fdf_read_time_stamp( fdf_file *bm, const bm_1d_template *templ,
                                  void *tstamp )
{
	unsigned int read_size = data_type_to_rw_size( templ->time_type );
	unsigned int bytes = fread( tstamp, read_size, 1, bm->f );
	return bytes;
}


/**
   \brief Casts and writes the data the correct way.
*/
unsigned int fdf_write_data_1d( fdf_file   *bm, const bm_1d_template *templ,
                                const void *data )
{
	unsigned int write_size = data_type_to_rw_size( templ->data_type );
	unsigned int bytes = fwrite( data, write_size, templ->N, bm->f );
	return bytes;
}



/**
   \brief Reads the time stamp the correct way.
*/
unsigned int fdf_read_data_1d( fdf_file *bm, const bm_1d_template *templ,
                               void *data )
{
	unsigned int read_size = data_type_to_rw_size( templ->data_type );
	unsigned int bytes = fread( data, read_size, templ->N, bm->f );
	return bytes;
}
