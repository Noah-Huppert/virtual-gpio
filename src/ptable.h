#ifndef __PTABLE_H__
#define __PTABLE_H__

#include "./port.h"

/**
 * Information for GPIO ports.
 */
typedef struct {
	/**
	 * Largest allowed port number
	 */
	int max_port_num;

	/**
	 * Directory in which GPIO port control files are located.
	 */
	char *control_f_dir;

	/**
	 * Holds exported GPIO ports.
	 * Indices are port numbers.
	 * If a value is NULL the port is not exported.
	 * Holds max_port_num count of GPIO ports.
	 */
	GPIOPort **ports;
} GPIOPortTable;

/**
 * Initializes a GPIO port table
 * @param max_port_num Number of ports table can hold
 * @param control_f_dir Directory in which GPIO port control files are located
 * @returns Allocated GPIOPortTable
 */
GPIOPortTable *ptable_init(int max_port_num, const char *control_f_dir);

/**
 * Frees a port table
 * @param ptable GPIO port table
 */
void ptable_free(GPIOPortTable *ptable);

/**
 * Exports a port.  
 *
 * Exits the program if an error occurs.
 *
 * @param ptable GPIO port table
 * @param number Port number
 */
void ptable_export(GPIOPortTable *ptable, int number);

/**
 * Unexports a port.
 *
 * Exits the program if an error occurs.
 *
 * @param ptable GPIO port table
 * @param number Port number
 */
void ptable_unexport(GPIOPortTable *ptable, int number);

/**
 * Set the state of the GPIO port table based on the actual directories 
 * present in the control_f_dir.
 *
 * Assumes the control_f_dir exists.
 *
 * Assumes that the files and directories in the control_f_dir have only been
 * modified by the program.
 *
 * If any of these assumptions does not hold the program may exit expectantly.
 *
 * Program will exit if any errors occur.
 *
 * @param ptable GPIO port table
 */
void ptable_restore(GPIOPortTable *ptable);

#endif
