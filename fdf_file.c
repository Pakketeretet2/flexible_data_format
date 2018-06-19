#include "fdf_file.h"
#include "fdf_data_types.h"
#include "fdf_types_full.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

/* Header for verification of integrity and version info */
static const char fdf_header[] = { '@', 'F', 'D', 'F', 13, 37, 0, 0,
                                   0, 0, 0, 1, 0, 0, 0, 0,
                                   'F', 'D', 'F', 'D', 'F', 'D', 'F', 'D',
                                   'F', 'D', 'F', 'D', 'F', 'D', 'F', '@' };


void fdf_close( fdf_file *bm )
{
	fclose(bm->f);
	free(bm);
}


fdf_file *fdf_open( const char *fname, int mode, int *status )
{
	fdf_file *bm = malloc( sizeof( fdf_file ) );
	bm->f = NULL;
	if( mode == FDF_READ_ONLY ){
		bm->f = fopen( fname, "rb" );
		if( !bm->f ){
			*status = FDF_OPEN_FOR_READ_FAILED;
		}else if( (*status = fdf_verify_header( bm )) != FDF_SUCCESS ){
			fclose( bm->f );
			bm->f = NULL;
		}else{
			// AOK.
			*status = FDF_SUCCESS;
		}
	}else if( mode == FDF_WRITE_ONLY ){
		bm->f = fopen( fname, "wb" );
		if( !bm->f ){
			*status = FDF_OPEN_FOR_WRITE_FAILED;
		}else{
			*status = FDF_SUCCESS;
		}
	}else{
		*status = FDF_UNKNOWN_WRITE_MODE;
	}
	return bm;
}



unsigned int fdf_write_header( fdf_file *fdf_f )
{
	unsigned int size_write = sizeof(char);
	unsigned int n_written = fwrite( (void*)fdf_header, size_write,
	                                 sizeof(fdf_header)/sizeof(char),
	                                 fdf_f->f );
	return n_written * size_write;
}


unsigned int fdf_read_header( fdf_file *fdf_f, char *header )
{
	unsigned int size_read = sizeof(char);
	unsigned int n_read = fread( (void*)header, size_read,
	                             sizeof(fdf_header), fdf_f->f );
	return n_read * size_read;;
}


int fdf_verify_header( fdf_file *fdf_f )
{
	char header[sizeof(fdf_header)];
	int bytes_read = fdf_read_header( fdf_f, header );
	if( bytes_read != sizeof(fdf_header) ){
		return FDF_READ_BYTES_MISMATCH;
	}
	for( int i = 0; i < sizeof(fdf_header)/sizeof(char); ++i ){
		if( header[i] != fdf_header[i] ){
			return FDF_CORRUPT_OR_NO_HEADER;
		}
	}
	return FDF_SUCCESS;
}


unsigned int fdf_write_template( fdf_file *f, const fdf_template *templ )
{
	unsigned int n_written = 0;
	unsigned int tot_bytes = 0;

	n_written = fdf_write_header( f );
	fprintf( stderr, "Header has a size of %lu chars.\n", sizeof(fdf_header) );
	assert( n_written == sizeof(fdf_header) && "Byte write mismatch!" );

	tot_bytes += n_written * sizeof(char);
	n_written = fwrite( templ, sizeof(fdf_template), 1, f->f );
	tot_bytes += n_written*sizeof(fdf_template);
	return tot_bytes;
}


unsigned int fdf_write_time( fdf_file *f, const fdf_template *templ,
			     void *tstamp )
{
	unsigned int size_write = fdf_data_type_to_rw_size( templ->time_type );
	unsigned int n_written = fwrite( tstamp, size_write, 1, f->f );
	assert( n_written == 1 &&
		"Byte write mismatch!" );
	return n_written * size_write;
}



unsigned int fdf_write_grid_meta( fdf_file *f, const fdf_grid_meta *grid_spec )
{
	unsigned int data_type = grid_spec->type;
	int Nx = grid_spec->size;

	assert( fdf_verify_data_type( data_type ) &&
	        "Unknown data type!" );

	unsigned int n_written = fwrite( &data_type, sizeof(unsigned int), 1, f->f );
	unsigned int tot_bytes = 0;

	assert( n_written == 1 &&
		"Byte write mismatch!" );
	tot_bytes += n_written*sizeof(int);

	n_written = fwrite( &Nx, sizeof(int), 1, f->f );
	assert( n_written == 1 &&
		"Byte write mismatch!" );
	tot_bytes += n_written*sizeof(unsigned int);
	return tot_bytes;
}


unsigned int fdf_write_grid_data( fdf_file *f, const fdf_grid_meta *grid_spec,
                                  void *grid )
{
	unsigned int size_write = fdf_data_type_to_rw_size( grid_spec->type );
	int n_written = fwrite( grid, size_write, grid_spec->size, f->f );
	assert( n_written == grid_spec->size &&
		"Byte write mismatch!" );
	int tot_bytes = n_written * size_write;
	return tot_bytes;
}


unsigned int fdf_write_data_1d( fdf_file *f, const fdf_template *templ,
				int Nx, void *data )
{
	assert( fdf_verify_data_type( templ->data_type ) &&
		"Unknown data type!" );
	unsigned int size_write = fdf_data_type_to_rw_size( templ->data_type );
	unsigned int n_written = fwrite( data, size_write, Nx, f->f );
	unsigned int bytes = n_written * size_write;

	assert( bytes == size_write*Nx &&
		"Byte write mismatch!" );
	return bytes;
}




int fdf_read_template( fdf_file *f, fdf_template *templ )
{
	unsigned int n_read = fread( templ, sizeof(fdf_template), 1, f->f );
	if( n_read != 1 ){
		return FDF_READ_BYTES_MISMATCH;
	}else{
		return FDF_SUCCESS;
	}
}


int fdf_read_time( fdf_file *f, const fdf_template *templ, void *tstamp )
{
	unsigned int size_read = fdf_data_type_to_rw_size( templ->time_type );
	unsigned int n_read = fread( tstamp, size_read, 1, f->f );
	if( n_read != 1 ){
		// Check if it was EOF or something else:
		if( feof(f->f) ){
			return FDF_EOF_REACHED;
		}else{
			return FDF_INVALID_READ;
		}
	}

	return FDF_SUCCESS;
}


int fdf_read_grid_meta( fdf_file *f, fdf_grid_meta *grid_specs )
{
	int n_read = fread( grid_specs, sizeof(fdf_grid_meta), 1, f->f );

	assert( n_read == 1 && "Byte read mismatch!" );
	return FDF_SUCCESS;
}



int fdf_read_grid_data( fdf_file *f, const fdf_grid_meta *grid_specs,
                        void *grid )
{
	unsigned int size_read = fdf_data_type_to_rw_size( grid_specs->type );
	int n_read = fread( grid, size_read, grid_specs->size, f->f );
	assert( n_read == grid_specs->size && "Byte read mismatch!" );
	return FDF_SUCCESS;
}


int fdf_read_data_1d( fdf_file *f, const fdf_template *templ, int N, void *data )
{
	int n_read = fdf_read_data_raw( f, templ, N, data );
	assert( n_read == N && "Byte read mismatch!" );
	return FDF_SUCCESS;
}




unsigned int fdf_write_data_2d( fdf_file *f, const fdf_template *templ,
				int Nx, int Ny, void *data )
{
	assert( fdf_verify_data_type( templ->data_type ) &&
		"Unknown data type!" );
	unsigned int size_write = fdf_data_type_to_rw_size( templ->data_type );
	unsigned int n_written = fwrite( data, size_write, Nx*Ny, f->f );
	unsigned int bytes = n_written * size_write;

	assert( bytes == size_write*Nx*Ny &&
		"Byte write mismatch!" );
	return bytes;
}



int fdf_read_data_2d( fdf_file *f, const fdf_template *templ,
		      int Nx, int Ny, void *data )
{
	int n_read = fdf_read_data_raw( f, templ, Nx*Ny, data );
	assert( n_read == Nx*Ny && "Byte read mismatch!" );
	return FDF_SUCCESS;
}



int fdf_read_data_raw( fdf_file *f, const fdf_template *templ,
                       int Ntot, void *data )
{
	unsigned int size_read = fdf_data_type_to_rw_size( templ->data_type );
	int n_read = fread( data, size_read, Ntot, f->f );
	assert( n_read == Ntot && "Byte read mismatch!" );
	return n_read;
}
