#include <stdlib.h>

#include "./port.h"

GPIOPort *gpio_port_init(int number) {
	GPIOPort *port = (GPIOPort*)malloc(sizeof(GPIOPort));

	port->number = number;
	port->direction = IN;
	port->value = false;

	return port;
}

void gpio_port_free(GPIOPort *port) {
	free(port);
}
