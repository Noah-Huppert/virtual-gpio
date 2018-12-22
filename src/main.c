#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * Print an error message and errno to stderr, and exit. 
 * @param err_msg Message with context of error
 */
void print_errno(const char *err_msg) {
	fprintf(stderr, "%s: %s\n", err_msg, strerror(errno));
	exit(1);
}

void main(int argc, char **argv) {
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
	}

	const char* control_f_dir = argv[1];

	// Create control file directory if it doesn't exist
	if (!stat(control_f_dir, NULL)) { // If failed to check
		print_errno("error checking if control file directory exists");
	} else if (ENOENT == errno) { // Doesn't exist
		// Create
		if (!mkdir(control_f_dir, 0777)) {
			print_errno("error creating control file directory");
		}
	}
}
