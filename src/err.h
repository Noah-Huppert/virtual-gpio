#ifndef __ERR_H__
#define __ERR_H__

/**
 * Print an error message and errno to stderr, and exit. 
 * @param err_msg Message with context of error
 */
void print_errno(const char *err_msg);

#endif
