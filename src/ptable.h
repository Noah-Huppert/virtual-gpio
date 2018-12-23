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
 * @returns Allocated GPIOPortTable
 */
GPIOPortTable *ptable_init(int max_port_num);

/**
 * Frees a port table
 * @param ptable GPIO port table
 */
void ptable_free(GPIOPortTable *ptable);

#endif
