#include "fdf_file.h"
#include "fdf_data_types.h"
#include "fdf_types_full.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* Magic header for file type and verification of integrity */
static const char magic_cookie[24] = { '@', 'F', 'D', 'F',
                                       13, 37, 0, 0,
                                       'F', 'D', 'F', 'D',
                                       'F', 'D', 'F', 'D',
                                       'F', 'D', 'F', 'D',
                                       'F', 'D', 'F', '@' };

struct checksum_uint128 {
	uint64_t s1, s2;
};

/**
   \brief The header that is present in each file.

   The header is 64 bits in size.
*/
struct fdf_header {
	/** The magic header. */
        char magic[24];         /* 40 bits left. */
	/** Version number, first 4 bits are major, last 4 bits are minor. */
	char version[8];        /* 32 bits left. */
	/** File size in bytes */
	uint64_t file_size;     /* 24 bits left. */
	/** MD5 Checksum */
	struct checksum_uint128 checksum;  /* 8 bits left. */

	/** Number of data blocks. */
	uint32_t n_blocks;      /* 4 bits left. */

	/** These are reserved for future use/unused.
	    Extension can always happen via the UTF-8 way, where
	    the last bit of unused will flag whether or not more
	    headers follow. Unused is guaranteed to be 0 for a 64 bit header.
	    for now is set to all zeros.
	 */
	char unused[4]; /* 4  bits left */
};




int fdf_write_file_meta( fdf_file *f );



void fdf_close( fdf_file *bm )
{
	/* Write some pending info to the header of the file. */
	fdf_write_file_meta( bm );

	fprintf( stderr, "Closing file at %p. Wrote %u bytes, "
	         "%d blocks to it.\n",
	         (void*)bm->f, bm->bytes_written, bm->blocks_written );

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
			/* AOK. */
			*status = FDF_SUCCESS;
			/* TODO: Set the checksum and other properties */

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
	struct fdf_header my_header;
	memcpy( my_header.magic, magic_cookie,
	        sizeof(magic_cookie)/sizeof(char) );
	my_header.version[0] = 0;
	my_header.version[1] = 0;
	my_header.version[2] = 0;
	my_header.version[3] = 1;
	my_header.version[4] = 0;
	my_header.version[5] = 0;
	my_header.version[6] = 0;
	my_header.version[7] = 1;

	/* Now figure out how you can seek back to the following bits
	   to write checksums and the like... */
	my_header.n_blocks  = 0;
	my_header.file_size = 8; /* Initial file size is 8 bytes. */
	my_header.checksum.s1 = 0;
	my_header.checksum.s2 = 0;
	my_header.unused[0] = 0;
	my_header.unused[1] = 0;
	my_header.unused[2] = 0;
	my_header.unused[3] = 0;

	unsigned int size_write = sizeof(struct fdf_header);
	unsigned int n_written = fwrite( &my_header, size_write, 1, fdf_f->f );
	assert( n_written * size_write == 8*my_header.file_size &&
	        "Programmer's assumption on file size incorrect!" );
	int bytes = n_written * size_write;
	fdf_f->bytes_written += bytes;
	return bytes;
}


unsigned int fdf_read_header( fdf_file *fdf_f, struct fdf_header *header )
{
	unsigned int size_read = sizeof(struct fdf_header);
	unsigned int n_read = fread( header, size_read, 1, fdf_f->f );
	return n_read * size_read;
}


int fdf_verify_header( fdf_file *fdf_f )
{
	struct fdf_header my_header;
	int bytes_read = fdf_read_header( fdf_f, &my_header );
	fprintf( stderr, "Read %d bytes.\n", bytes_read );
	if( bytes_read != sizeof(struct fdf_header) ){
		return FDF_READ_BYTES_MISMATCH;
	}

	/* Various checks on the header: */
	if( fdf_verify_header_cookie( &my_header ) == 0 ){
		return FDF_CORRUPT_OR_NO_HEADER;
	}
	return FDF_SUCCESS;
}


int fdf_verify_header_cookie( const void *header )
{
	const struct fdf_header *my_header = header;
	for( int i = 0; i < sizeof(my_header->magic); ++i ){
		if( my_header->magic[i] != magic_cookie[i] ){
			return 0;
		}
	}
	return 1;
}



unsigned int fdf_write_template( fdf_file *f, const fdf_template *templ )
{
	unsigned int tot_bytes = 0;

	tot_bytes = fdf_write_header( f );
	long unsigned int header_size = sizeof(struct fdf_header);
	long unsigned int template_size = sizeof(struct fdf_template);
	fprintf( stderr, "Header has a size of %lu chars.\n", header_size );
	assert( tot_bytes == header_size && "Byte write mismatch!" );

	int n_written = fwrite( templ, sizeof(fdf_template), 1, f->f );
	unsigned int bytes_template = n_written * sizeof(fdf_template);
	tot_bytes += bytes_template;
	assert( tot_bytes == header_size + template_size
	        && "Byte write mismatch!" );

	f->bytes_written += bytes_template;
	return tot_bytes;
}


unsigned int fdf_write_time( fdf_file *f, const fdf_template *templ,
			     void *tstamp )
{
	unsigned int size_write = fdf_data_type_to_rw_size( templ->time_type );
	unsigned int n_written = fwrite( tstamp, size_write, 1, f->f );
	assert( n_written == 1 &&
		"Byte write mismatch!" );

	unsigned int bytes = n_written * size_write;
	f->bytes_written += bytes;
	return bytes;
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
	f->bytes_written += tot_bytes;
	return tot_bytes;
}


unsigned int fdf_write_grid_data( fdf_file *f, const fdf_grid_meta *grid_spec,
                                  void *grid )
{
	unsigned int size_write = fdf_data_type_to_rw_size( grid_spec->type );
	int n_written = fwrite( grid, size_write, grid_spec->size, f->f );
	assert( n_written == grid_spec->size && "Byte write mismatch!" );
	unsigned int tot_bytes = n_written * size_write;
	f->bytes_written += tot_bytes;
	return tot_bytes;
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
	return n_read;
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


int fdf_read_data_raw( fdf_file *f, const fdf_template *templ,
                       const fdf_grid_meta *grid_specs, void *data )
{
	unsigned int size_read = fdf_data_type_to_rw_size( templ->data_type );
	int n_elements = grid_specs[0].size;
	if( templ->dimension == 2 ){
		n_elements *= grid_specs[1].size;
	}
	int n_read = fread( data, size_read, n_elements, f->f );
	assert( n_read == n_elements && "Byte read mismatch!" );
	return n_read;
}



unsigned int fdf_write_data_raw( fdf_file *f, const fdf_template *templ,
                                 const fdf_grid_meta *grid_specs, void *data )
{
	unsigned int size_write = fdf_data_type_to_rw_size( templ->data_type );
	int n_elements = grid_specs[0].size;
	if( templ->dimension == 2 ){
		n_elements *= grid_specs[1].size;
	}

	int n_write = fwrite( data, size_write, n_elements, f->f );
	assert( n_write == n_elements && "Byte read mismatch!" );
	f->bytes_written += n_elements*size_write;
	return n_elements*size_write;
}




unsigned int fdf_write_data_block( fdf_file *f, const fdf_template *templ,
                                   const fdf_grid_meta *grid_specs,
                                   void *time, void *data )
{
	unsigned int bytes = 0;
	bytes += fdf_write_time( f, templ, time );
	bytes += fdf_write_data_raw( f, templ, grid_specs, data );

	f->blocks_written++;

	return bytes;
}



int fdf_read_data_block( fdf_file *f, const fdf_template *templ,
                         const fdf_grid_meta *grid_specs,
                         void *time, void *data )
{
	int n_read = 0;
	n_read += fdf_read_time( f, templ, time );
	if( n_read != 1 ){
		return 0;
	}
	int n_expect = grid_specs[0].size;
	if( templ->dimension == 2 ){
		n_expect *= grid_specs[1].size;
	}
	n_read += fdf_read_data_raw( f, templ, grid_specs, data );
	if( n_read != n_expect + 1 ){
		return 0;
	}
	return n_read;
}



int fdf_getpos( fdf_file *f, fpos_t *pos )
{
	return fgetpos( f->f, pos );
}

int fdf_jump_to_pos( fdf_file *f, const fpos_t *pos )
{
	return fsetpos( f->f, pos );
}

int fdf_jump_to_n_blocks( fdf_file *f )
{
	return fseek( f->f, 24+8, SEEK_SET );
}

int fdf_jump_to_file_size( fdf_file *f )
{
	return fseek( f->f, 24+8+4, SEEK_SET );
}

int fdf_jump_to_checksum( fdf_file *f )
{
	return fseek( f->f, 24+8+4+8, SEEK_SET );
}


void fdf_calc_checksum( fdf_file *f, struct checksum_uint128 *chk )
{
	chk->s1 = 0;
	chk->s2 = 0;
}



int fdf_write_file_meta( fdf_file *f )
{
	/* It is easiest to jump back to top and work your way down. */
	fpos_t end_of_file;
	int status = fdf_getpos( f, &end_of_file );
	if( !status ){
		return FDF_FSEEK_ERROR;
	}

	fdf_jump_to_file_size( f );
	int bytes = fwrite( &f->bytes_written, sizeof(uint64_t), 1, f->f );

	struct checksum_uint128 checksum;
	fdf_calc_checksum( f, &checksum );

	/* Do NOT count these in bytes written because they
	   overwrite data. */
	bytes += fwrite( &checksum, sizeof(checksum), 1, f->f );
	bytes += fwrite( &f->blocks_written, sizeof(uint32_t), 1, f->f );

	fprintf( stderr, "(Re)wrote %d bytes to file header.\n",
	        bytes );
	fdf_jump_to_pos( f, &end_of_file );

	return bytes;
}
