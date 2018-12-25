#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#include "./fifo.h"
#include "./err.h"

int open_or_mk_fifo(char *f_path) {
	struct stat stat_buff;

	// Check if file exists
	if (stat(f_path, &stat_buff) < 0) { // If error while checking
		if (ENOENT == errno) { // Doesn't exist
			// Create
			if (mkfifo(f_path, 0666) < 0) {
				char err[1000];
				snprintf(err, sizeof(err), "error creating FIFO file: %s",
						 f_path);

				print_errno(err);
			}
		} else { // Other error
			char err[1000];
			snprintf(err, sizeof(err), "error checking if file exists: %s",
					 f_path);

			print_errno(err);
		}
	}

	// Open
	int fd = open(f_path, O_NONBLOCK);

	if (fd < 0) { // Error
		char err[1000];
		snprintf(err, sizeof(err), "error opening FIFO file: %s", f_path);

		print_errno(err);
	}

	return fd;
}
