#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "./err.h"

void print_errno(const char *err_msg) {
	fprintf(stderr, "%s: %s\n", err_msg, strerror(errno));
	exit(1);
}
