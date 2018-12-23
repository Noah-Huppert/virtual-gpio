#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
		       "    virtual-gpio CONTROL_FILE_DIR\n"
			   "\n"
			   "OPTIONS\n"
			   "\n"
			   "    --help,-h    Print this help text\n"
		       "\n"
		       "ARGUMENTS\n"
		       "\n"
		       "    CONTROL_FILE_DIR    Directory to place virtual GPIO\n"
		"                        control files\n");
		exit(0);
	}

	const char* control_f_dir = argv[1];

	vgpio = vgpio_init(control_f_dir);

	atexit(cleanup);

	return 0;
}
