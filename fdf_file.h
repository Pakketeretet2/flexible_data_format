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
typedef struct fdf_file {
	FILE *f;
} fdf_file;




/**
   \brief Open a FDF file for writing or reading.
*/
fdf_file *fdf_open( const char *fname, const char *mode );


/**
   \brief Close a FDF file.
*/
void fdf_close( fdf_file *f );





/* 2-D data: */




#endif // FDF_FILE_H
