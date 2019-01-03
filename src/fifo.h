#ifndef __FIFO_H__
#define __FIFO_H__

#include <fcntl.h>

/**
 * Opens or creates a fifo file and returns the file descriptor.
 *
 * Exits on error.
 *
 * @param f_path File path
 * @returns File descriptor
 */
int open_or_mk_fifo(char *f_path);

/**
 * Creates a fifo file if it doesn't exist
 *
 * Exits the program on error.
 *
 * @param f_path File path
 */
void mkfifo_if_no_exist(char *f_path);

#endif
