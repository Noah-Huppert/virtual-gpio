#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "./vgpio.h"
#include "./err.h"
#include "./fifo.h"

VirtualGPIO *vgpio_init(const char *control_f_dir, int max_port_num) {
	// TODO: Check process has enough file descriptors available for the number
	// of specified ports
	// Allocate
	VirtualGPIO *vgpio = (VirtualGPIO*)malloc(sizeof(VirtualGPIO));

	vgpio->control_f_dir = control_f_dir;

	vgpio->ptable = ptable_init(max_port_num, vgpio->control_f_dir);

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

	vgpio->export_r_fd = open_or_mk_fifo(export_path);

	// Create unexport control file
	int unexport_path_len = strlen(vgpio->control_f_dir);
	unexport_path_len += 9; // + for "/unexport"

	char unexport_path[unexport_path_len];
	sprintf(unexport_path, "%s/unexport", vgpio->control_f_dir);

	vgpio->unexport_r_fd = open_or_mk_fifo(unexport_path);

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

	// Free port table
	ptable_free(vgpio->ptable);

	// Free rest of struct
	free(vgpio);
}

void vgpio_run(VirtualGPIO *vgpio) {
	//ptable_unexport(vgpio->ptable, 1);
}
