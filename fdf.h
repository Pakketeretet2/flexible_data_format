#ifndef FDF_H
#define FDF_H


/**
   \file fdf.h This is a small C library that provides support for reading and
   writing matrix-like data files that provide efficient storage.

   The format of FDF files is as follows:
   [header] [template] [other]
   [header]   is a simple 4-char header consisting of "FDF0" to
              indicate the file type.
   [template] describes the data. It is a struct with three ints:
       { time_type, data_type, fixed_grid_size }.
       time_type encodes the data type of the time variable
       data_type encodes the data type of the data itself
       fixed_grid_size is 1 if the data has a constant grid for all time points

   The contents of other depend on whether or not fixed_grid_size is 1 of 0.
   If it is 1, then other starts with
   [dim]
   which indicates the number of dimensions of the data.
   Then, there are dim entries of
   [dim size][type][grid]
   for which
   [dim size]  is the size of the dimth grid
   [type]      is the type of data of the dimth grid
   [grid]      is the dimth grid
   Finally, there are N entries of [time] [data]
   where
   [time]      is the time step corresponding to the data and
   [data]      is the data itself
*/


#include "fdf_types_opaque.h"
#include "fdf_data_types.h"
#include "fdf_enums.h"
#include "fdf_template.h"
#include "fdf_file.h"


int fdf_read_fixed_grid_meta( fdf_file *f, const fdf_template *templ,
			      int *dimension, void ***grids, int **grid_sizes,
			      unsigned int **grid_types );




#endif // FDF_H
