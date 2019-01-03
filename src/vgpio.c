#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>

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

	sprintf(vgpio->export_f_path, "%s/export", vgpio->control_f_dir);

	mkfifo_if_no_exist(vgpio->export_f_path);

	// Create unexport control file
	int unexport_path_len = strlen(vgpio->control_f_dir);
	unexport_path_len += 9; // + for "/unexport"

	sprintf(vgpio->unexport_f_path, "%s/unexport", vgpio->control_f_dir);

	mkfifo_if_no_exist(vgpio->unexport_f_path);

	return vgpio;
}

void vgpio_free(VirtualGPIO *vgpio) {
	// Free port table
	ptable_free(vgpio->ptable);

	// Free rest of struct
	free(vgpio);
}

void vgpio_run(VirtualGPIO *vgpio) {
	// Open export fifo
	int export_fd = open(vgpio->export_f_path, O_RDWR);
	if (export_fd < 0) { // Failed to open
		print_errno("error opening export fifo");
	}

	// Read or write fifo until "quit" is in buffer	
	while (true) {
		fd_set read_fds;
		fd_set write_fds;

		FD_ZERO(&read_fds);
		FD_SET(export_fd, &read_fds);

		FD_ZERO(&write_fds);
		FD_SET(export_fd, &write_fds);

		int num_fds = select(export_fd+1, &read_fds, &write_fds, NULL, NULL);
		if (num_fds < 0) { // Failed to select
			print_errno("failed to select export fifo fds");
		} else if (num_fds == 0) { // Timeout
			continue;
		}

		// If read
		if (FD_ISSET(export_fd, &read_fds)) {
			printf("on read\n");

			char buf[1000] = "";

			if (read(export_fd, buf, sizeof(buf)) < 0) {
				print_errno("failed to read from export fd");
			}

			printf("read: \"%s\"\n", buf);
		}

		// If write
		if (FD_ISSET(export_fd, &write_fds)) {
			printf("on write\n");

			char *buf = "on write\n";

			if (write(export_fd, buf, sizeof(buf)) < 0) {
				print_errno("failed to write to export fd");
			}

			printf("wrote\n");
		}
		break;
	}

	// Close export fifo
	if (close(export_fd) < 0) { // Failed to close
		print_errno("failed to close export fifo");
	}
}
