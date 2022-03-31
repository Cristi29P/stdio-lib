//
// Created by cristian on 29.03.2022.
//
#include "so_stdio.h"
#include "useful.h"
#include <stdio.h>

struct _so_file {
	int fd; /* The associated file descriptor */
	int file_pointer; /* Cursor position inside file. */
	int buffer_pointer; /* Cursor position inside buffer */
	char buffer[BUFF_SIZE]; /* The associated file buffer used for I/O tasks */
	char file_name[PATH_LENGTH];
	char opening_mode[MODE_LENGTH];
	char error; /* Flag set if error occurred during file I/O tasks */
	char eof_reached;
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
	f->error = 0;
	return f;
}

int so_fclose(SO_FILE *stream) {
	close(stream->fd);
	free(stream);

	return 0;
}

int so_fileno(SO_FILE *stream) {
	return stream->fd;
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
	int total_nr_of_bytes = (int)(size * nmemb);
	int bytes_read = 0;

	char *buf = calloc(total_nr_of_bytes, sizeof(char));

	if (!buf) {
		return 0;
	}

	while (bytes_read < total_nr_of_bytes) {
		char ch = (char)so_fgetc(stream);

		if (so_feof(stream) || so_ferror(stream)) {
			break;
		}

		buf[bytes_read++] = ch;
	}

	memcpy(ptr, buf, bytes_read);
	free(buf);
	return (bytes_read / size);
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	return 0;
}


int so_fgetc(SO_FILE *stream) {
	/* We check if the file is opened under read mode */
	if (readRight(stream->opening_mode)) {
		if (stream->buffer_pointer == (int)strnlen(stream->buffer, BUFF_SIZE)) {
			memset(stream->buffer, '\0', BUFF_SIZE); /* Make sure the buffer is clean */
			int rv = (int)read(stream->fd, stream->buffer, BUFF_SIZE);

			if (!rv) {
				stream->eof_reached = 1;
				return SO_EOF;
			}

			if (rv < 0) {
				stream->error = 1;
				return SO_EOF;
			}

			stream->buffer_pointer = 0; /* Make sure we read from the beginning of the buffer */
		}
		stream->file_pointer++;

		unsigned char c = stream->buffer[stream->buffer_pointer++];
		return (int) c;
	}

	stream->error = 1; /* We couldn't read a character, so we have an error */
	return SO_EOF;
}

int so_fputc(int c, SO_FILE *stream) {
	return 0;
}

int so_feof(SO_FILE *stream) {
	return stream->eof_reached;
}

int so_ferror(SO_FILE *stream) {
	return stream->error;
}

SO_FILE *so_popen(const char *command, const char *type) {
	return NULL;
}

int so_pclose(SO_FILE *stream) {
	return 0;
}
