#ifndef __VGPIO_H__
#define __VGPIO_H__

/**
 * Holds information related to the virtual GPIO system's state.
 */
typedef struct {
	/**
	 * Directory control files are located in.
	 */
	const char *control_f_dir;

	/**
	 * File descriptor for reading the export control file.
	 */
	int export_r_fd;

	/**
	 * File descriptor for reading the unexport control file.
	 */
	int unexport_r_fd;
} VirtualGPIO;

/**
 * Initializes a VirtualGPIO struct and opens / creates related control
 * files / directories.
 *
 * Exits program if an error occurs.
 *
 * @param control_f_dir Control file directory
 */
VirtualGPIO *vgpio_init(const char *control_f_dir);

/**
 * Closes file descriptors and frees a VirtualGPIO struct.
 */
void vpgio_free(VirtualGPIO *vpgio);

#endif
