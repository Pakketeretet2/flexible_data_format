#ifndef FDF_TYPES_FULL_H
#define FDF_TYPES_FULL_H

#include <stdio.h>


struct fdf_file
{
	FILE *f;
};


struct fdf_template
{
	int time_type;
	int data_type;
	int dimension;
	int fixed_grid_size;
};


struct fdf_grid_meta
{
	unsigned int type;
	int size;
};





#endif // FDF_TYPES_FULL_H
