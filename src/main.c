#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "./vgpio.h"

VirtualGPIO *vgpio;

void cleanup() {
	vgpio_free(vgpio);
}

int main(int argc, char **argv) {
	// Check for command line arguments
	if (argc < 2 || strcmp(argv[1], "-h") == 0 ||
			strcmp(argv[1], "--help") == 0) {
		// Print help if no command line arguments
		printf("virtual-gpio - Emulates a Linux GPIO system\n"
			   "\n"
		       "USAGE\n"
		       "\n"
		       "    virtual-gpio CONTROL_FILE_DIR [MAX_PORT_NUM]\n"
			   "\n"
			   "OPTIONS\n"
			   "\n"
			   "    --help,-h    Print this help text\n"
		       "\n"
		       "ARGUMENTS\n"
		       "\n"
		       "    CONTROL_FILE_DIR    Directory to place virtual GPIO\n"
			   "                        control files\n"
			   "\n"
			   "    MAX_PORT_NUM        Maximum allowed port number, \n"
			   "                        optional, defaults to 99\n");
		exit(0);
	}

	const char* control_f_dir = argv[1];
	long int max_port_num = 99;

	// .... If max port num arg provided
	if (argc > 2) {
		char *end_pntr;

		max_port_num = strtol(argv[2], &end_pntr, 10);

		if (argv[2] == end_pntr) { // No number in string
			fprintf(stderr, "error parsing MAX_PORT_NUM argument into "
					"integer: \"%s\"\n", argv[2]);
			exit(1);
		}
	}

	if (max_port_num < 0) {
		fprintf(stderr, "MAX_PORT_NUM argument cannot be negative\n");
		exit(1);
	}

	// Initialize virtual GPIO
	vgpio = vgpio_init(control_f_dir, max_port_num);

	atexit(cleanup);

	// Run virtual GPIO
	vgpio_run(vgpio);

	return 0;
}
