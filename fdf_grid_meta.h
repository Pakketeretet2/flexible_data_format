#ifndef FDF_GRID_META_H
#define FDF_GRID_META_H


#include "fdf_types_opaque.h"


fdf_grid_meta *fdf_grid_meta_init();

void fdf_grid_meta_destroy( fdf_grid_meta * );


unsigned int fdf_grid_meta_get_type( const fdf_grid_meta * );

int fdf_grid_meta_set_type( fdf_grid_meta *, unsigned int );

int fdf_grid_meta_get_size( const fdf_grid_meta * );

int fdf_grid_meta_set_size( fdf_grid_meta *, int );





#endif // FDF_GRID_META_H
