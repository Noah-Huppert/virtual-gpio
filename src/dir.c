#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>

#include "./dir.h"
#include "./err.h"

/**
 * Called for each item in the file tree
 * @param fpath Path to item
 * @param stat_buff Info from stat call on item
 * @param typeflag Indicates the type of the item
 * @param ftw_buff Holds information about depth in file tree
 * @returns Non zero value to exit
 */
int walk(const char *fpath, const struct stat *stat_buff, int typeflag,
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
	if (nftw(dir_path, walk, 10, FTW_DEPTH) != 0) {
		fprintf(stderr, "failed to walk directory: \"%s\"\n", dir_path);
		exit(0);
	}
}
