#include "fdf_data_2d.h"

/**
   \brief Sets size in template to given value.
*/
void fdf_2d_template_set_size( bm_2d_template *, unsigned int );

/**
   \brief Sets time type in template to given type.
*/
void fdf_2d_template_set_time_type( bm_2d_template *, unsigned int );

/**
   \brief Sets data type in template to given type.
*/
void fdf_2d_template_set_data_type( bm_2d_template *, unsigned int );


/**
   \brief Returns the size of the data corresponding to the template.
*/
unsigned int fdf_2d_template_get_size( const bm_2d_template * );

/**
   \brief Returns the time type of the template.
*/
unsigned int fdf_2d_template_get_time_type( const bm_2d_template * );

/**
   \brief Returns the data type of the template
*/
unsigned int fdf_2d_template_get_data_type( const bm_2d_template * );



/**
   \brief Initializes bm_2d_template to default values
*/
void fdf_2d_template_init( bm_2d_template * );


/**
   \brief Writes one-dimensional data to FDF file.
*/
int fdf_write_2d( fdf_file *, const bm_2d_template *,
                  const void *tstamp, const void *data );


/**
   \brief Reads one-dimensional data from FDF file.
*/
int fdf_read_2d( fdf_file *, bm_2d_template *,
                 void *tstamp, void *data );
