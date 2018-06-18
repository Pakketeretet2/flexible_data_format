#ifndef FDF_DATA_TYPES_H
#define FDF_DATA_TYPES_H


/**
   \brief enum that encodes for supported data types.
*/
enum fdf_data_types {
	UNSET             = 0,
	FDF_DATA_INT32    = 1,
	FDF_DATA_INT64    = 2,
	FDF_DATA_DOUBLE   = 10
};


/**
   \brief Checks if the given data type is valid.

   Returns 1 if type is valid, 0 otherwise.
*/
int fdf_verify_data_type( unsigned int );

#endif // FDF_DATA_TYPES_H
