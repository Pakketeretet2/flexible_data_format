This is an example binary data format that I plan to use to store data in 
such a way that it does not take up much room, can be read/written quickly,
and is still flexible.

This repository consists of the basic library that implements low-level
functions for reading and writing FDF files as well as some higher-level
API calls that are easier for the end user.

This is half to be useful, half to learn some more about nitty gritty file
manipulation in C.

main.c contains some usag examples.

TODO:
 - Add Doxygen comments
 - Implement a checksum on the file body
 - Expand the higher-level interface
 - Write C++ bindings
