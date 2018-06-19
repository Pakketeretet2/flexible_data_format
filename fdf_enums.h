#ifndef FDF_ENUMS_H
#define FDF_ENUMS_H


/**
   \brief Read and write modes:
*/
enum read_write_modes
{
	/* Failed to allocate memory. */
	FDF_MALLOC_FAILED = -100,

	/* At EOF */
	FDF_EOF_REACHED = -10,

	/* Read/write errors */
	FDF_INVALID_READ = -4,
	FDF_OPEN_FOR_READ_FAILED = -3,
	FDF_OPEN_FOR_WRITE_FAILED = -2,
	FDF_UNKNOWN_WRITE_MODE = -1,

	/* AOK */
	FDF_SUCCESS = 0,

	/* Read/write modes. */
	FDF_READ_ONLY = 1,
	FDF_WRITE_ONLY = 2,

	/* Errors relating to invalid reads. */
	FDF_READ_DIMENSION_MISMATCH = 10,
	FDF_READ_BYTES_MISMATCH = 11,
	FDF_CORRUPT_OR_NO_HEADER = 12,

	/* Errors relating to sizes, types, memory, etc. */
	FDF_FILE_NOT_FIXED_GRID = 20,
	FDF_INCORRECT_DIMENSION = 21,
	FDF_INVALID_GRID_SIZE   = 22,
	FDF_UNKNOWN_DATA_TYPE   = 23
};


#endif // FDF_ENUMS_H
