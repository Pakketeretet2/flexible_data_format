#include "fdf_data_1d.h"
#include "fdf_file.h"
#include "fdf_data_types.h"

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
	fdf_write_data_1d( bm, templ, data );

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
	fdf_read_data_1d( bm, templ, data );

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
