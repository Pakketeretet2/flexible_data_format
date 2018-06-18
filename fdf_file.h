#ifndef FDF_FILE_H
#define FDF_FILE_H

/**
   \file fdf_file.h
   Contains definitions for opening and closing fdf files, which is a simple
   wrapper around a FILE pointer.
*/


#include <stdio.h>


/**
   \brief IO error codes:
*/
enum io_error_codes
{
	SUCCESS = 0,
	READ_DIMENSION_MISMATCH = 1
};


/**
   \brief Simple opaque pointer.
*/
typedef struct  {
	FILE *f;
} fdf_file;



/**
   \brief This is a template that describes 1D data.

   \note Follow pthreads API
*/
typedef struct {
	int N;
	int time_type;
	int data_type;
} bm_1d_template;



/**
   \brief Open a FDF file for writing or reading.
*/
fdf_file *fdf_open( const char *fname, const char *mode );


/**
   \brief Close a FDF file.
*/
void fdf_close( fdf_file *f );



/**
   \brief Sets size in template to given value.
*/
void fdf_1d_template_set_size( bm_1d_template *, unsigned int );

/**
   \brief Sets time type in template to given type.
*/
void fdf_1d_template_set_time_type( bm_1d_template *, unsigned int );

/**
   \brief Sets data type in template to given type.
*/
void fdf_1d_template_set_data_type( bm_1d_template *, unsigned int );


/**
   \brief Returns the size of the data corresponding to the template.
*/
unsigned int fdf_1d_template_get_size( const bm_1d_template * );

/**
   \brief Returns the time type of the template.
*/
unsigned int fdf_1d_template_get_time_type( const bm_1d_template * );

/**
   \brief Returns the data type of the template
*/
unsigned int fdf_1d_template_get_data_type( const bm_1d_template * );



/**
   \brief Initializes bm_1d_template to default values
*/
void fdf_1d_template_init( bm_1d_template * );


/**
   \brief Writes one-dimensional data to FDF file.
*/
int fdf_write_1d( fdf_file *, const bm_1d_template *,
                  const void *tstamp, const void *data );


/**
   \brief Reads one-dimensional data from FDF file.
*/
int fdf_read_1d( fdf_file *, bm_1d_template *,
                 void *tstamp, void *data );





#endif // FDF_FILE_H
