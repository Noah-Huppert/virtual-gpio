#ifndef __VGPIO_H__
#define __VGPIO_H__

#include "./ptable.h"

/**
 * Holds information related to the virtual GPIO system's state.
 */
typedef struct {
	/**
	 * Directory control files are located in.
	 */
	const char *control_f_dir;

	/**
	 * Path to the export control file.
	 */
	char export_f_path[1000];

	/**
	 * Path to the unexport control file.
	 */
	char unexport_f_path[1000];

	/**
	 * Holds exported GPIO ports.
	 */
	GPIOPortTable *ptable;
} VirtualGPIO;

/**
 * Initializes a VirtualGPIO struct and opens / creates related control
 * files / directories.
 *
 * Exits program if an error occurs.
 *
 * @param control_f_dir Control file directory
 * @param max_port_num Largest allowed port number
 */
VirtualGPIO *vgpio_init(const char *control_f_dir, int max_port_num);

/**
 * Closes file descriptors and frees a VirtualGPIO struct.
 * @param vgpio Virtual GPIO
 */
void vgpio_free(VirtualGPIO *vgpio);

/**
 * Runs the main virtual GPIO logic loop which emulates a GPIO system.
 *
 * Exits program if an error occurs.
 *
 * @param vgpio Virtual GPIO
 */
void vgpio_run(VirtualGPIO *vgpio);

#endif
