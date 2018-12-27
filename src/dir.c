#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "./dir.h"
#include "./err.h"

/**
 * Called for each item in the file tree and deletes it.
 * @param fpath Path to item
 * @param stat_buff Info from stat call on item
 * @param typeflag Indicates the type of the item
 * @param ftw_buff Holds information about depth in file tree
 * @returns Non zero value to exit
 */
int rm_rf_walk(const char *fpath, const struct stat *stat_buff, int typeflag,
		  struct FTW *ftw_buff) {
	// Delete item
	if (remove(fpath) < 0) { // Error
		char err[1000];
		snprintf(err, sizeof(err), "error deleting file: \"%s\"", fpath);

		print_errno(err);
	}

	return 0;
}

void rm_rf(char *dir_path) {
	if (nftw(dir_path, rm_rf_walk, 10, FTW_DEPTH) != 0) {
		fprintf(stderr, "failed to walk directory: \"%s\"\n", dir_path);
		exit(0);
	}
}

char **ls(char *dir_path) {
	char **file_names = (char**)malloc(sizeof(char*)*5);
	for (int i = 0; i < 5; i++) {
		file_names[i] = NULL;
	}

	int loaded_names = 0;

	int start_errno = errno;
	
	// Open directory
	DIR *dir = opendir(dir_path);
	if (dir == NULL) { // Error opening directory
		char err[1000];
		snprintf(err, sizeof(err), "failed to open directory for ls: %s",
				dir_path);
		print_errno(err);
	}

	// Get dir entries
	struct dirent *dir_entry = readdir(dir);

	while (dir_entry != NULL) {
		// Check if reached max number of file names
		if (loaded_names == 5) {
			fprintf(stderr, "failed to list file names, more than 5 entries "
					"encountered, path: %s\n", dir_path);
			exit(1);
		}

		// Check file name is not current directory or parent directory
		if (strcmp(dir_entry->d_name, ".") != 0 && 
			strcmp(dir_entry->d_name, "..") != 0) {

			// Record file name
			loaded_names++;
			file_names[loaded_names-1] = (char*)malloc(sizeof(char)*1000);
			strncpy(file_names[loaded_names-1], dir_entry->d_name,
					sizeof(char)*1000);
		}

		// Call again
		dir_entry = readdir(dir);
	}

	if (errno != start_errno) { // Error occurred while reading directory
		char err[1000];
		snprintf(err, sizeof(err), "error listing entries in path %s",
				dir_path);
		print_errno(err);
	}

	// Close directory
	if (closedir(dir) < 0) { // Failed to close directory
		char err[1000];
		snprintf(err, sizeof(err), "error closing directory for ls: %s",
				 dir_path);
		print_errno(err);
	}

	// Success
	return file_names;
}

void ls_free(char **file_names) {
	for (int i = 0; i < 5; i++) {
		// If not filled
		if (file_names[i] == NULL) {
			// Done freeing
			return;
		}

		// Free
		free(file_names[i]);
	}

	// Free array
	free(file_names);
}
