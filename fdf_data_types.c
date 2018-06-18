#include "fdf_data_types.h"


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
