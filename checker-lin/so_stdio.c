//
// Created by cristian on 29.03.2022.
//
#include "so_stdio.h"
#include "useful.h"

struct _so_file {
	int fd; /* The associated file descriptor */
	int file_pointer; /* Cursor position inside file. */
	int buffer_pointer; /* Cursor position inside buffer */
	char buffer[BUFF_SIZE]; /* The associated file buffer used for I/O tasks */
	char file_name[PATH_LENGTH];
	char opening_mode[MODE_LENGTH];
};

SO_FILE *so_fopen(const char *pathname, const char *mode) {
	SO_FILE *f = NULL;
	int flags = get_flags(mode);

	if (flags == INVALID_FLAGS) {
		return NULL;
	}

	int fd = open(pathname, flags, 0644);
	if (fd < 0) {
		return NULL;
	}

	f = calloc(1, sizeof(SO_FILE));
	if (f == NULL) {
		return NULL;
	}

	f->fd = fd;
	f->file_pointer = 0;
	f->buffer_pointer = 0;
	memset(f->buffer, '\0', BUFF_SIZE);
	strncpy(f->file_name, pathname, PATH_LENGTH);
	strncpy(f->opening_mode, mode, MODE_LENGTH);

	return f;
}

int so_fclose(SO_FILE *stream) {
	close(stream->fd);
	free(stream);

	return 0;
}

int so_fileno(SO_FILE *stream) {
	return 0;
}

int so_fflush(SO_FILE *stream) {
	return 0;
}

int so_fseek(SO_FILE *stream, long offset, int whence) {
	return 0;
}

long so_ftell(SO_FILE *stream) {
	return 0;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	return 0;
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	return 0;
}

int so_fgetc(SO_FILE *stream) {
	return 0;
}

int so_fputc(int c, SO_FILE *stream) {
	return 0;
}

int so_feof(SO_FILE *stream) {
	return 0;
}

int so_ferror(SO_FILE *stream) {
	return 0;
}

SO_FILE *so_popen(const char *command, const char *type) {
	return NULL;
}

int so_pclose(SO_FILE *stream) {
	return 0;
}
