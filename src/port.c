#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "./port.h"
#include "./err.h"
#include "./fifo.h"
#include "./dir.h"

GPIOPort *gpio_port_init(int number, char *all_control_f_dir) {
	GPIOPort *port = (GPIOPort*)malloc(sizeof(GPIOPort));

	port->number = number;
	port->direction = IN;
	port->value = false;

	size_t control_f_size = sizeof(char)*1000;
	port->control_f_dir = (char*)malloc(control_f_size);
	snprintf(port->control_f_dir, control_f_size, "%s/gpio%d", 
			 all_control_f_dir, port->number);

	return port;
}

void gpio_port_free(GPIOPort *port) {
	// Close direction control file 
	if (close(port->direction_fd) < 0) {
		char err[1000];
		snprintf(err, sizeof(err), "error closing direction control file "
				 "for port number: %d", port->number);

		print_errno(err);
	}

	// Close value control file 
	if (close(port->value_fd) < 0) {
		char err[1000];
		snprintf(err, sizeof(err), "error closing value control file "
				 "for port number: %d", port->number);

		print_errno(err);
	}

	// Free
	free(port->control_f_dir);
	free(port);
}

void gpio_port_open_control_files(GPIOPort *port) {
	// TODO: Use inotify_add_watch instead of fifo
	// Create direction control file
	char direction_f_path[1000];
	snprintf(direction_f_path, sizeof(direction_f_path), "%s/direction",
			 port->control_f_dir);

	port->direction_fd = open_or_mk_fifo(direction_f_path);

	// Create value control file
	char value_f_path[1000];
	snprintf(value_f_path, sizeof(value_f_path), "%s/value",
			 port->control_f_dir);

	port->value_fd = open_or_mk_fifo(value_f_path);
}

void gpio_port_export(GPIOPort *port) {
	// Create control file directory
	struct stat stat_buff;

	if (stat(port->control_f_dir, &stat_buff) < 0) { // Error checking dir
		if (errno == ENOENT) {
			// If control file dir doesn't exist, create
			if (mkdir(port->control_f_dir, 0777) < 0) { // Error creating dir
				char err[1000];
				snprintf(err, sizeof(err),
						 "error creating GPIO control file directory for "
						 "port number: %d", port->number);

				print_errno(err);
			}
		} else { // Other error
			char err[1000];
			snprintf(err, sizeof(err), "error checking if GPIO port directory "
					 "exists for port number: %d", port->number);

			print_errno(err);
		}
	}

	// Open control files
	gpio_port_open_control_files(port);
}

void gpio_port_unexport(GPIOPort *port) {
	// Check if control file directory exists
	struct stat stat_buff;

	if (stat(port->control_f_dir, &stat_buff) < 0) { // Error checking dir
		if (errno != ENOENT) {
			// If control file dir doesn't exist, no exported
			return;
		} else { // Other error
			char err[1000];
			snprintf(err, sizeof(err), "error checking if GPIO port directory "
					 "exists for port number: %d", port->number);

			print_errno(err);
		}
	}

	// Delete control file directory
	rm_rf(port->control_f_dir);
}
