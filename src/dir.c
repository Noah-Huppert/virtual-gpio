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

void ls(char *dir_path, char ***entry_names, int *entries_capacity) {
	// Allocate entry_names
	*entries_capacity = LS_ENTRIES_REALLOC_SIZE;
	int loaded_entries = 0;

	*entry_names = (char**)malloc(sizeof(char*)*(*entries_capacity));
	for (int i = 0; i < (*entries_capacity); i++) {
		(*entry_names)[i] = NULL;
	}
	
	// Open directory
	DIR *dir = opendir(dir_path);
	if (dir == NULL) { // Error opening directory
		char err[1000];
		snprintf(err, sizeof(err), "failed to open directory for ls: %s",
				dir_path);
		print_errno(err);
	}

	// Record starting errno to determine if any readdir calls fail
	int start_errno = errno;

	// Get dir entries
	struct dirent *dir_entry = readdir(dir);

	while (dir_entry != NULL) {
		// Check entry name is not current directory or parent directory
		if (strcmp(dir_entry->d_name, ".") != 0 && 
			strcmp(dir_entry->d_name, "..") != 0) {

			// Check if reached max number of entry names
			if (loaded_entries == (*entries_capacity)) {
				// Reallocate
				*entries_capacity += LS_ENTRIES_REALLOC_SIZE;

				int new_size = sizeof(char*)*(*entries_capacity);
				(*entry_names) = (char**)realloc((*entry_names), new_size);

				// Set new entries to NULL
				for (int i = (*entries_capacity) - LS_ENTRIES_REALLOC_SIZE;
					 i < (*entries_capacity); i++) {
					(*entry_names)[i] = NULL;
				}
			}

			// Record entry name
			loaded_entries++;
			int entry_i = loaded_entries-1;
			(*entry_names)[entry_i] = (char*)malloc(sizeof(char)
					                             *MAX_LS_ENTRY_LENGTH);

			strncpy((*entry_names)[entry_i], dir_entry->d_name,
					sizeof(char)*MAX_LS_ENTRY_LENGTH);
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
}

void ls_free(char **entry_names, int entries_capacity) {
	for (int i = 0; i < entries_capacity; i++) {
		// If not filled
		if (entry_names[i] == NULL) {
			// Done freeing
			return;
		}

		// Free
		free(entry_names[i]);
	}

	// Free array
	free(entry_names);
}
