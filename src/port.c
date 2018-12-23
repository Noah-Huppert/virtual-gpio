#include <stdlib.h>
#include <sys/stat.h>

#include "./port.h"

GPIOPort *gpio_port_init(int number, char *all_control_f_dir) {
	GPIOPort *port = (GPIOPort*)malloc(sizeof(GPIOPort));

	port->number = number;
	port->direction = IN;
	port->value = false;

	port->control_f_dir = (char*)malloc(sizeof(char)*1000);
	snprintf(port->control_f_dir, sizeof(port->control_f_dir),
			 "%s/gpio%d", all_control_f_dir, port->number);

	return port;
}

void gpio_port_free(GPIOPort *port) {
	free(port->control_f_dir);
	free(port);
}

void gpio_port_export(GPIOPort *port) {
	// Check if port is already exported
	struct stat stat_buff;

	// TODO: Implment gpio_port_export
//	if (stat())
}
