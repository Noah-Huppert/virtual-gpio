#include "./ptable.h"

#include <stdlib.h>

GPIOPortTable *ptable_init(int max_port_num, char *control_f_dir) {
	// Allocate
	GPIOPortTable *ptable = (GPIOPortTable*)malloc(sizeof(GPIOPortTable));

	ptable->max_port_num = max_port_num;
	ptable->control_f_dir = control_f_dir;

	// Setup table
	ptable->ports = (GPIOPort**)malloc(sizeof(GPIOPort)*ptable->max_port_num);

	// ... Set all port pointers to NULL to indicate they are unexported
	for (int i = 0; i < ptable->max_port_num; i++) {
		ptable->ports[i] = NULL;
	}

	return ptable;
}

void ptable_free(GPIOPortTable *ptable) {
	// Free ports
	for (int i = 0; i < ptable->max_port_num; i++) {
		GPIOPort *port = ptable->ports[i];

		// If exported
		if (port != NULL) {
			gpio_port_free(port);
		}
	}

	// Free struct
	free(ptable);
}
