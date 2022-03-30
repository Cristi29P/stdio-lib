//
// Created by cristian on 30.03.2022.
//

#include "useful.h"

int get_flags(const char *mode) {
	int flags = INVALID_FLAGS;

	if (!strncmp(mode, "r", 1)) {
		flags = O_RDONLY;
	}

	if (!strncmp(mode, "r+", 2)) {
		flags = O_RDWR;
	}

	if (!strncmp(mode, "w", 1)) {
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	}

	if (!strncmp(mode, "w+", 2)) {
		flags = O_RDWR | O_TRUNC | O_CREAT;
	}

	if (!strncmp(mode, "a", 1)) {
		flags = O_APPEND | O_CREAT | O_WRONLY;
	}

	if (!strncmp(mode, "a+", 2)) {
		flags = O_APPEND | O_CREAT | O_RDWR;
	}

	return flags;
}