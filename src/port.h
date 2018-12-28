#ifndef __PORT_H__
#define __PORT_H__

#include <stdbool.h>

/**
 * GPIO port signal direction
 */
typedef enum {
	IN,
	OUT,
} GPIOPortDirection;


/**
 * Holds information about an open GPIO port
 */
typedef struct {
	/**
	 * Identifying number
	 */
	int number;

	/**
	 * Directory port control files are located in
	 */
	char *control_f_dir;

	/**
	 * Signal direction
	 */
	GPIOPortDirection direction;

	/**
	 * Direction control file descriptor
	 */
	int direction_fd;

	/**
	 * Value
	 */
	bool value;

	/**
	 * Value control file descriptor
	 */
	int value_fd;
} GPIOPort;

/**
 * Creates a GPIOPort
 * @param number Identifying number
 * @param all_control_f_dir Directory in which control files for all GPIO 
 *                      ports are located.
 * @returns GPIOPort pointer
 */
GPIOPort *gpio_port_init(int number, char *all_control_f_dir);

/**
 * Frees a GPIOPort
 * @param port GPIO port
 */
void gpio_port_free(GPIOPort *port);

/**
 * Opens a port's control file descriptors
 *
 * Exits program if an error occurs.
 *
 * @param port GPIO port
 */
void gpio_port_open_control_files(GPIOPort *port);

/**
 * Export a port.
 * Creates a GPIO port control directory and control files.
 *
 * Exits program if an error occurs.
 *
 * @param port GPIO port
 */
void gpio_port_export(GPIOPort *port);

/**
 * Unexport a port.
 * Delete GPIO port control directory.
 *
 * Exits program if an error occurs.
 *
 * @param port GPIO port
 */
void gpio_port_unexport(GPIOPort *port);

#endif
