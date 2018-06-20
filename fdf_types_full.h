#ifndef FDF_TYPES_FULL_H
#define FDF_TYPES_FULL_H

#include <stdio.h>
#include <stdint.h>

struct fdf_file
{
	FILE *f;
	uint64_t bytes_written;
	uint32_t blocks_written;
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
