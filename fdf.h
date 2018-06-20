#ifndef FDF_H
#define FDF_H


/**
   \file fdf.h This is a small C library that provides support for reading and
   writing matrix-like data files that provide efficient storage.

   The format of FDF files is as follows:
   [header] [template] [other]
   [header]   is a simple magic header that indicates the file type
              and serves as integrity check
   [template] describes the data. It is a struct with four ints:
       { time_type, data_type, dim, fixed_grid_size }.
       time_type encodes the data type of the time variable
       data_type encodes the data type of the data itself
       dim       encodes the dimensionality of the data (1D, 2D, ... )
       fixed_grid_size is 1 if the data has a constant grid for all time points


   The contents of other depend on whether or not fixed_grid_size is 1 of 0.
   If it is 1, then the rest of the file is laid out as
   dim x [type][dim size]
   dim x [grid]
   N   x [time][data]

   With: [dim size]  The size of the dim-th grid
         [type]      The type of the dim-th grid
         [grid]      The dim-th grid
         [time]      The time corresponding to the data
         [data]      the actual data.
   where the first corresponds to the first encounter of [dim size], etc.


   if fixed_grid_size is 0, then the rest of the file is laid out as follows:

*/

#include "fdf_types_opaque.h"
#include "fdf_data_types.h"
#include "fdf_enums.h"
#include "fdf_template.h"
#include "fdf_grid_meta.h"
#include "fdf_file.h"



int fdf_init_grid_meta( const fdf_template *templ,
                        fdf_grid_meta ***grid_specs );

int fdf_init_grid( const fdf_template *templ,
                   fdf_grid_meta **grid_specs, void ***grids );

int fdf_init_data( const fdf_template *templ,
                   const fdf_grid_meta *grid_specs, void **data );


int fdf_destroy_grid_meta( fdf_grid_meta *grid_specs );

int fdf_destroy_grid( const fdf_template *templ, void **grids );

int fdf_destroy_data( void *data );


int fdf_read_grid_block( fdf_file *f, const fdf_template *templ,
                         const fdf_grid_meta *grid_specs, void *time,
                         void *data );





#endif // FDF_H
