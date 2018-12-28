#ifndef __DIR_H__
#define __DIR_H__

/**
 * Remove directory and it's contents
 * @param dir_path Path of directory to remove
 */
void rm_rf(char *dir_path);

/**
 * The number of entries which will be added to the entry_names array if it 
 * has reached capacity and needs to be reallocated.
 */
#define LS_ENTRIES_REALLOC_SIZE 10

/**
 * Maximum size of entry file names returned by ls()
 */
#define MAX_LS_ENTRY_LENGTH 1000

/**
 * Returns a list of file names in a directory.
 *
 * Exits the program if an error occurs.
 *
 * If a file name is longer than MAX_LS_ENTRY_LENGTH characters it will
 * be truncated.
 *
 * @param dir_path Path of directory to list
 * @param entry_names Pointer to array of character arrays which will hold
 *                    entry names
 * @param entries_capacity Pointer to variable which will hold the number of
 *                         entry names which the entry_names array can hold.
 *                         This does not indicate that all these entries are
 *                         used. Just the size of the array.
 * @returns Array of file names in
 *          shape char[MAX_LS_ENTRIES][MAX_LS_ENTRY_LENGTH]. Entries will be
 *          NULL if they are not filled. Must be freed with ls_free().
 */
void ls(char *dir_path, char ***entry_names, int *entries_capacity);

/**
 * Frees the memory returned by ls().
 * @param entry_names Array of entry names
 * @param entries_capacity Number of entries which entry_names can hold
 */
void ls_free(char **entry_names, int entries_capacity);

#endif
