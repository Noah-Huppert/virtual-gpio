#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "./ptable.h"
#include "./port.h"
#include "./dir.h"

GPIOPortTable *ptable_init(int max_port_num, const char *control_f_dir) {
	// Allocate
	GPIOPortTable *ptable = (GPIOPortTable*)malloc(sizeof(GPIOPortTable));

	ptable->max_port_num = max_port_num;
	ptable->control_f_dir = (char*) control_f_dir;

	// Setup table
	//     max_port_num + 1 b/c port numbers start at 0
	ptable->ports = (GPIOPort**)malloc(sizeof(GPIOPort)*ptable->max_port_num+1);

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
	ptable->ports[number] = gpio_port_init(number, ptable->control_f_dir);

	// Export port
	gpio_port_export(ptable->ports[number]);
}

void ptable_unexport(GPIOPortTable *ptable, int number) {
	// Check if already unexported
	if (ptable->ports[number-1] == NULL) {
		fprintf(stderr, "port already unexported: %d\n", number);
		exit(1);
	}

	// Unexport port
	gpio_port_unexport(ptable->ports[number]);

	// Deallocate
	free(ptable->ports[number]);
	ptable->ports[number] = NULL;
}

void ptable_restore(GPIOPortTable *ptable) {
	// Get names of entries in control file directory
	char **control_f_entries = NULL;
	int control_f_entries_cap = 0;

	ls(ptable->control_f_dir, &control_f_entries, &control_f_entries_cap);

	// Check to see if any entry names are GPIO port control file directories
	for (int i = 0; i < control_f_entries_cap; i++) {
		// If reached last full entry
		if (control_f_entries[i] == NULL) {
			break;
		}

		// Check if entry name is long enough to hold "gpio<port number>"
		char *entry_name = control_f_entries[i];
		int entry_name_l = strlen(entry_name);

		if (entry_name_l <= 4) { // Entry name not long enough
			continue;
		}

		// Check if entry name in format "gpio<port number>"
		bool all_numbers = true;

		for (int j = 5; j < entry_name_l; j++) { // Check each char in name
			if (!isdigit(entry_name[j])) { // If char not a digit
				all_numbers = false;
				break;
			}
		}

		if (!all_numbers) { // If not a valid number
			continue;
		}

		// Parse port number into int
		int port_number = atoi(entry_name[5]);

		// Initialize in ports array
		ptable->ports[port_number] = gpio_port_init(port_number,
				                                    ptable->control_f_dir);
	}

	// Free control directory entry names
	ls_free(control_f_entries, control_f_entries_cap);
}
