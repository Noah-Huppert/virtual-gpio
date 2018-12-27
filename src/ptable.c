#include <stdlib.h>
#include <stdio.h>

#include "./ptable.h"
#include "./port.h"

GPIOPortTable *ptable_init(int max_port_num, const char *control_f_dir) {
	// Allocate
	GPIOPortTable *ptable = (GPIOPortTable*)malloc(sizeof(GPIOPortTable));

	ptable->max_port_num = max_port_num;
	ptable->control_f_dir = (char*) control_f_dir;

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

void ptable_export(GPIOPortTable *ptable, int number) {
	// Check if already exported
	if (ptable->ports[number-1] != NULL) {
		fprintf(stderr, "port already exported: %d\n", number);
		exit(1);
	}

	// Initialize port
	ptable->ports[number-1] = gpio_port_init(number, ptable->control_f_dir);

	// Export port
	gpio_port_export(ptable->ports[number-1]);
}

void ptable_unexport(GPIOPortTable *ptable, int number) {
	// Check if already unexported
	if (ptable->ports[number-1] == NULL) {
		fprintf(stderr, "port already unexported: %d\n", number);
		exit(1);
	}

	// Unexport port
	gpio_port_unexport(ptable->ports[number-1]);

	// Deallocate
	free(ptable->ports[number-1]);
	ptable->ports[number-1] = NULL;
}
