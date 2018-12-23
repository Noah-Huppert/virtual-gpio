#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "./vgpio.h"
#include "./err.h"


const char *mkfifo_stat_err = "error checking if file exists: ";
const char *mkfifo_create_err = "error creating FIFO file: ";
const char *mkfifo_open_err = "error opening FIFO file: ";

/**
 * Opens or creates a fifo file and returns the file descriptor.
 *
 * Exits on error.
 *
 * @param f_path File path
 * @param mode File open mode
 * @returns File descriptor
 */
int open_or_mk_fifo(char *f_path, mode_t mode) {
	struct stat stat_buff;

	// Check if file exists
	if (stat(f_path, &stat_buff) < 0) { // If error while checking
		if (ENOENT == errno) { // Doesn't exist
			// Create
			if (mkfifo(f_path, 0666) < 0) {
				int err_len = strlen(mkfifo_create_err);
				err_len += strlen(f_path);

				char err[err_len];
				sprintf(err, "%s%s", mkfifo_create_err, f_path);

				print_errno(err);
			}
		} else { // Other error
			int err_len = strlen(mkfifo_stat_err);
			err_len += strlen(f_path);

			char err[err_len];
			sprintf(err, "%s%s", mkfifo_stat_err, f_path);

			print_errno(err);
		}
	}

	// Open
	int fd = open(f_path, mode | O_NONBLOCK);

	if (fd < 0) { // Error
		int err_len = strlen(mkfifo_open_err);
		err_len += strlen(f_path);

		char err[err_len];
		sprintf(err, "%s%s", mkfifo_open_err, f_path);

		print_errno(err);
	}

	return fd;
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
	int export_path_len = strlen(vgpio->control_f_dir);
	export_path_len += 7; // + for "/export"

	char export_path[export_path_len];
	sprintf(export_path, "%s/export", vgpio->control_f_dir);

	vgpio->export_r_fd = open_or_mk_fifo(export_path, O_RDONLY);

	// Create unexport control file
	int unexport_path_len = strlen(vgpio->control_f_dir);
	unexport_path_len += 9; // + for "/unexport"

	char unexport_path[unexport_path_len];
	sprintf(unexport_path, "%s/unexport", vgpio->control_f_dir);

	vgpio->unexport_r_fd = open_or_mk_fifo(unexport_path, O_RDONLY);

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
