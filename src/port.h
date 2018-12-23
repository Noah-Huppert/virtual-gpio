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
	 * Signal direction
	 */
	GPIOPortDirection direction;

	/**
	 * Value
	 */
	bool value;
} GPIOPort;

/**
 * Creates a GPIOPort
 * @param number Identifying number
 * @returns GPIOPort pointer
 */
GPIOPort *gpio_port_init(int number);

/**
 * Frees a GPIOPort
 * @param port GPIO port
 */
void gpio_port_free(GPIOPort *port);

#endif
