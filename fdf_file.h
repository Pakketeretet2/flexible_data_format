#ifndef FDF_FILE_H
#define FDF_FILE_H

/**
   \file fdf_file.h
   Contains definitions for opening and closing fdf files, which is a simple
   wrapper around a FILE pointer.
*/


#include "fdf_types_opaque.h"
#include "fdf_enums.h"



/**
   \brief Open a FDF file for writing or reading.
*/
fdf_file *fdf_open( const char *fname, int mode, int *status );


/**
   \brief Writes a header to the given fdf file.
*/
unsigned int fdf_write_header( struct fdf_file *f );

/**
   \brief Verifies that fdf file has the right header.
*/
int fdf_verify_header( fdf_file *fdf_f );


/**
   \brief Close a FDF file.
*/
void fdf_close( fdf_file *f );





/**
   \brief Writes the template to the file.
*/
unsigned int fdf_write_template( fdf_file *f, const fdf_template *templ );


unsigned int fdf_write_time( fdf_file *f, const fdf_template *templ,
			     void *tstamp );

unsigned int fdf_write_grid_meta( fdf_file *f, const fdf_grid_meta *grid_spec );

unsigned int fdf_write_grid_data( fdf_file *f, const fdf_grid_meta *grid_spec,
                                  void *grid );




unsigned int fdf_write_data_1d( fdf_file *f, const fdf_template *templ,
				int Nx, void *data );



/**
   \brief Reads the template from file.
*/
int fdf_read_template( fdf_file *f, fdf_template *templ );

int fdf_read_time( fdf_file *f, const fdf_template *templ, void *tstamp );


int fdf_read_grid_meta( fdf_file *f, fdf_grid_meta *grid_spec );


int fdf_read_grid_data( fdf_file *f, const fdf_grid_meta *grid_specs,
                        void *grid );


int fdf_read_data_1d( fdf_file *f, const fdf_template *templ,
                      int N, void *data );


unsigned int fdf_write_data_2d( fdf_file *f, const fdf_template *templ,
				int Nx, int Ny, void *data );


int fdf_read_data_2d( fdf_file *f, const fdf_template *templ,
		      int Nx, int Ny, void *data );


int fdf_read_data_raw( fdf_file *f, const fdf_template *templ,
                       int Ntot, void *data );



#endif // FDF_FILE_H
