#ifndef __DIR_H__
#define __DIR_H__

/**
 * Remove directory and it's contents
 * @param dir_path Path of directory to remove
 */
void rm_rf(char *dir_path);

/**
 * Returns a list of file names in a directory.
 *
 * Exits the program if an error occurs.
 *
 * Exits the program if the directory has more than 5 entries. This is because 
 * the function is only used to check GPIO port control directories which
 * should only have 2 files.
 *
 * If a file name is longer than 1000 characters it will be truncated.
 *
 * @param dir_path Path of directory to list
 * @returns Array of file names in shape char[5][1000]. Entries will be NULL 
 *          if they are not filled. Must be freed with ls_free().
 */
char **ls(char *dir_path);

/**
 * Frees the memory returned by ls().
 * @param file_names Array of file names.
 */
void ls_free(char **file_names);

#endif
