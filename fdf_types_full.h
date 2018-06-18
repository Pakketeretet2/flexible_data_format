#ifndef FDF_TYPES_FULL_H
#define FDF_TYPES_FULL_H

#include <stdio.h>


struct fdf_file
{
	FILE *f;
	int nwrites;
};


struct fdf_template
{
	int time_type;
	int data_type;
	int fixed_grid_size;
};









#endif // FDF_TYPES_FULL_H
