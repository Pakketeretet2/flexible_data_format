#ifndef FDF_TEMPLATE_H
#define FDF_TEMPLATE_H

#include "fdf_types_opaque.h"

/**
   \brief Initialize FDF template.
*/
fdf_template *fdf_template_init();


/**
   \brief Destroy FDF template.
*/
void fdf_template_destroy( fdf_template * );


/**
   \brief Sets the time type of the template.
*/
int fdf_template_set_time_type( fdf_template *templ, unsigned int type );


/**
   \brief Returns the time type of the template.
*/
unsigned int fdf_template_get_time_type( const fdf_template *templ );


/**
   \brief Sets the data type of the template.
*/
int fdf_template_set_data_type( fdf_template *templ, unsigned int type );

/**
   \brief Returns the data type of the template.
*/
unsigned int fdf_template_get_data_type( const fdf_template *templ );


/**
   \brief Sets whether or not the written data is for a fixed grid.
*/
void fdf_template_set_fixed_grid( fdf_template *templ, unsigned int type );


/**
   \brief Returns whether or not the written data is for a fixed grid.
*/
int fdf_template_is_fixed_grid( fdf_template *templ );


/**
   \brief Sets the data type of the template.
*/
int fdf_template_set_dimension( fdf_template *templ, int dim );

/**
   \brief Returns the data type of the template.
*/
int fdf_template_get_dimension( const fdf_template *templ );




#endif //  FDF_TEMPLATE_H
