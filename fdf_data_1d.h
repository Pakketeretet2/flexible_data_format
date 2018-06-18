#ifndef FDF_1D_DATA_H
#define FDF_1D_DATA_H


/* Forward declare file struct: */
typedef struct fdf_file fdf_file;


/**
   \brief This is a template that describes 1D data.
*/
typedef struct bm_1d_template {
	int N;
	int time_type;
	int data_type;
} bm_1d_template;


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


#endif // FDF_1D_DATA_H
