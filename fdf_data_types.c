#include "fdf_data_types.h"

#include <stdint.h>

int fdf_verify_data_type( unsigned int type )
{
	switch(type){
		case FDF_DATA_INT32:
		case FDF_DATA_INT64:
		case FDF_DATA_DOUBLE:
			return 1;
		default:
			return 0;
	}
}



unsigned int fdf_data_type_to_rw_size( unsigned int data_type )
{
	switch( data_type ){
		case FDF_DATA_INT32:
			return sizeof(int32_t);
		case FDF_DATA_INT64:
			return sizeof(int64_t);
		case FDF_DATA_DOUBLE:
			return sizeof(double);
		default:
			// Error?
			// assert( false && "Unknown data type!" );
			return 0;
	}
}


unsigned int fdf_data_type_to_ptr_size( unsigned int data_type )
{
	switch( data_type ){
		case FDF_DATA_INT32:
			return sizeof(int32_t*);
		case FDF_DATA_INT64:
			return sizeof(int64_t*);
		case FDF_DATA_DOUBLE:
			return sizeof(double*);
		default:
			// Error?
			// assert( false && "Unknown data type!" );
			return 0;
	}
}
