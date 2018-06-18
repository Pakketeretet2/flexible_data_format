#include "fdf_file.h"
#include "fdf_data_types.h"
#include "fdf_types_full.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>


void fdf_close( fdf_file *bm )
{
	fclose(bm->f);
	free(bm);
}


fdf_file *fdf_open( const char *fname, int mode, int *status )
{
	fdf_file *bm = malloc( sizeof( fdf_file ) );
	bm->f = NULL;
	bm->nwrites = 0;
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
	const char* header = "FDF0";
	unsigned int size_write = sizeof(char);
	unsigned int n_written = fwrite( (void*)header, size_write, 4, fdf_f->f );
	return n_written * size_write;
}


unsigned int fdf_read_header( fdf_file *fdf_f, char *header )
{
	unsigned int size_read = sizeof(char);
	unsigned int n_written = fread( (void*)header, size_read, 4, fdf_f->f );
	return n_written;
}


int fdf_verify_header( fdf_file *fdf_f )
{
	char header[4];
	unsigned int n_written = fdf_read_header( fdf_f, header );
	if( n_written != 4*sizeof(char) ){
		return FDF_READ_BYTES_MISMATCH;
	}
	if( header[0] == 'F' && header[1] == 'D' &&
	    header[2] == 'F' && header[3] == '0' ){
		// OK.
		return FDF_SUCCESS;
	}else{
		return FDF_CORRUPT_OR_NO_HEADER;
	}
}


unsigned int fdf_write_template( fdf_file *f, const fdf_template *templ )
{
	unsigned int n_written = 0;
	unsigned int tot_bytes = 0;

	if( f->nwrites == 0 ){
		n_written = fdf_write_header( f );
	}
	assert( n_written == 4 && "Byte write mismatch!" );
	tot_bytes += n_written * sizeof(char);
	f->nwrites++;
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



unsigned int fdf_write_grid( fdf_file *f, unsigned int data_type,
			     int Nx, void *grid )
{
	assert( fdf_verify_data_type( data_type ) &&
		"Unknown data type!" );
	unsigned int size_write = fdf_data_type_to_rw_size( data_type );
	unsigned int n_written = fwrite( &Nx, sizeof(int), 1, f->f );
	unsigned int tot_bytes = 0;

	assert( n_written == 1 &&
		"Byte write mismatch!" );
	tot_bytes += n_written*sizeof(int);

	n_written = fwrite( &data_type, sizeof(unsigned int), 1, f->f );
	assert( n_written == 1 &&
		"Byte write mismatch!" );
	tot_bytes += n_written*sizeof(unsigned int);

	n_written = fwrite( grid, size_write, Nx, f->f );
	assert( n_written == Nx &&
		"Byte write mismatch!" );
	tot_bytes += n_written * size_write;
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



unsigned int fdf_write_dimension( fdf_file *f, int dimension )
{
	assert( ( (dimension == 1) || (dimension == 2) ) &&
		"Dimensions other than 1 or 2 are not supported!" );

	unsigned int n_written = fwrite( &dimension, sizeof(int), 1, f->f );
	fprintf( stderr, "Wrote %u elements.\n",  n_written );
	unsigned int bytes = n_written * sizeof(int);
	assert( bytes == sizeof(int) &&
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
	assert( n_read ==1 && "Byte read mismatch!" );
	return FDF_SUCCESS;
}


int fdf_read_grid_meta( fdf_file *f, int *Nx, unsigned int *grid_type )
{
	int n_read = fread( Nx, sizeof(int), 1, f->f );
	assert( n_read == 1 && "Byte read mismatch!" );

	n_read = fread( grid_type, sizeof(unsigned int), 1, f->f );
	assert( n_read == 1 && "Byte read mismatch!" );
	return FDF_SUCCESS;
}



int fdf_read_grid( fdf_file *f, unsigned int data_type, int Nx, void *grid )
{
	unsigned int size_read = fdf_data_type_to_rw_size( data_type );
	int n_read = fread( grid, size_read, Nx, f->f );
	assert( n_read == Nx && "Byte read mismatch!" );
	return FDF_SUCCESS;
}


int fdf_read_data_1d( fdf_file *f, const fdf_template *templ, int N, void *data )
{
	unsigned int size_read = fdf_data_type_to_rw_size( templ->data_type );
	int n_read = fread( data, size_read, N, f->f );
	assert( n_read == N && "Byte read mismatch!" );
	return FDF_SUCCESS;
}


int fdf_read_dimension( fdf_file *f, int *dimension )
{
	int n_read = fread( dimension, sizeof(int), 1, f->f );
	assert( n_read == 1 && "Byte read mismatch!" );
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
	unsigned int size_read = fdf_data_type_to_rw_size( templ->data_type );
	int n_read = fread( data, size_read, Nx*Ny, f->f );
	assert( n_read == Nx*Ny && "Byte read mismatch!" );
	return n_read * size_read;
}
