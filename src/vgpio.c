#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "./vgpio.h"
#include "./err.h"

/**
 * Opens or creates a fifo file and returns the file descriptor.
 * @param f_path File path
 * @returns File descriptor
 */
int open_or_mk_fifo(char *f_path) {
	struct stat stat_buff;

	// Check if file exists
	if (stat(f_path, &stat_buff) < 0) { // If error while checking
		if (ENOENT == errno) { // Doesn't exist
		} else { // Other error
			// TODO finish open_or_mk_fifo
			print_errno()
		}
	}
}

VirtualGPIO *vgpio_init(const char *control_f_dir) {
	VirtualGPIO *vgpio = (VirtualGPIO*)malloc(sizeof(VirtualGPIO));
	vgpio->control_f_dir = control_f_dir;

	// Create control file directory if it doesn't exist
	struct stat stat_buff;

	if (stat(vgpio->control_f_dir, &stat_buff) < 0) { // If failed to check
		if (ENOENT == errno) { // Doesn't exist
			// Create
			if (mkdir(vgpio->control_f_dir, 0777) < 0) {
				print_errno("error creating control file directory");
			}
		} else { // Other error
			print_errno("error checking if control file directory exists");
		}
	}

	// Create export control file
	// TODO use open_or_mk_fifo for export control file
	int export_path_len = strlen(vgpio->control_f_dir);
	export_path_len += 7; // + for "/export"

	char export_path[export_path_len];
	sprintf(export_path, "%s/export", vgpio->control_f_dir);

	vgpio->export_r_fd = open(export_path, O_RDONLY | O_CREAT, 0666);

	if(vgpio->export_r_fd < 0) {
		print_errno("error opening export control file");
	}

	// Create unexport control file
	// TODO use open_or_mk_fifo for unexport control file
	int unexport_path_len = strlen(vgpio->control_f_dir);
	unexport_path_len += 9; // + for "/unexport"

	char unexport_path[unexport_path_len];
	sprintf(unexport_path, "%s/unexport", vgpio->control_f_dir);

	vgpio->unexport_r_fd = open(unexport_path, O_RDONLY | O_CREAT, 0666);

	if (vgpio->unexport_r_fd < 0) {
		print_errno("error opening unexport control file");
	}

	return vgpio;
}

void vgpio_free(VirtualGPIO *vgpio) {
	// Close file descriptors
	if (close(vgpio->export_r_fd) < 0) {
		print_errno("failed to close export control file");
	}

	if (close(vgpio->unexport_r_fd) < 0) {
		print_errno("failed to close unexport control file");
	}

	free(vgpio);
}
