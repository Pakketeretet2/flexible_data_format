#ifndef FDF_ENUMS_H
#define FDF_ENUMS_H


enum success
{
	FDF_SUCCESS = 0
};


/**
   \brief IO error codes:
*/
enum io_error_codes
{
	FDF_READ_DIMENSION_MISMATCH = 1,
	FDF_READ_BYTES_MISMATCH = 2,
	FDF_CORRUPT_OR_NO_HEADER = 3
};


/**
   \brief data set/get error codes:
*/
enum data_error_codes
{
	FDF_UNKNOWN_DATA_TYPE = 1
};


/**
   \brief Read and write modes:
*/
enum read_write_modes
{
	FDF_OPEN_FOR_READ_FAILED = -3,
	FDF_OPEN_FOR_WRITE_FAILED = -2,
	FDF_UNKNOWN_WRITE_MODE = -1,
	FDF_READ_ONLY = 1,
	FDF_WRITE_ONLY = 2
};

/**
   \brief High level error codes:
*/
enum high_level_errors {
	FDF_FILE_NOT_FIXED_GRID = 1
};


#endif // FDF_ENUMS_H
