#include "so_stdio.h"
#include "useful.h"

struct _so_file {
	int fd; /* The associated file descriptor */
	int file_pointer; /* Cursor position inside file. */
	int buffer_pointer; /* Cursor position inside buffer */
	int last_bytes; /* Last number of bytes added to the buffer*/
	int last_action; /* Last I/O operation on the buffer */
	int error; /* Flag set if error occurred during I/O tasks */
	int eof_reached; /* Flag set if eof reached during I/O tasks */
	char opening_mode[MODE_LENGTH]; /* File read/write/append permissions */
	char buffer[BUFF_SIZE]; /* The associated file buffer used for I/O tasks */
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
	f->last_bytes = 0;
	memset(f->buffer, '\0', BUFF_SIZE);
	strncpy(f->opening_mode, mode, MODE_LENGTH);
	f->error = 0;
	return f;
}

int so_fclose(SO_FILE *stream) {
	if (stream->last_action == WRITE_OPERATION) {
		so_fflush(stream);
	}


	int rt = close(stream->fd);
	free(stream);

	return rt;
}

int so_fileno(SO_FILE *stream) {
	return stream->fd;
}

int so_fflush(SO_FILE *stream) {
	if (stream->last_action == WRITE_OPERATION) {
		if (write(stream->fd, stream->buffer, stream->buffer_pointer) < 0) {
			stream->error = !stream->error;
			return SO_EOF;
		}

		memset(stream->buffer, '\0', BUFF_SIZE);
		stream->buffer_pointer = 0;
		return 0;
	}
	return SO_EOF;
}

int so_fseek(SO_FILE *stream, long offset, int whence) {
	return 0;
}

long so_ftell(SO_FILE *stream) {
	return 0;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	int bytes_read = 0;

	while (bytes_read < (int) (size * nmemb)) {
		char ch = (char) so_fgetc(stream);
		if (so_feof(stream) || so_ferror(stream)) {
			break;
		}
		((char *) ptr)[bytes_read++] = ch;
	}

	return (bytes_read / size);
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream) {
	int bytes_written = 0;

	while (bytes_written < (int) (size * nmemb)) {
		so_fputc((int) ((char *) ptr)[bytes_written++], stream);
		if (so_feof(stream) || so_ferror(stream)) {
			break;
		}

	}

	return (bytes_written / size);
}


int so_fgetc(SO_FILE *stream) {
	/* We check if the file is opened under read mode */
	if (readRight(stream->opening_mode)) {
		if (stream->buffer_pointer == stream->last_bytes) {
			memset(stream->buffer, '\0', BUFF_SIZE); /* Make sure the buffer is clean */
			int rv = (int) read(stream->fd, stream->buffer, BUFF_SIZE);

			if (!rv) {
				stream->eof_reached = !stream->eof_reached;
				return SO_EOF;
			}

			if (rv < 0) {
				stream->error = !stream->error;
				return SO_EOF;
			}

			stream->last_bytes = rv;
			stream->buffer_pointer = 0; /* Make sure we read from the beginning of the buffer */
		}
		stream->file_pointer++;
		stream->last_action = READ_OPERATION;
		return (int) stream->buffer[stream->buffer_pointer++];
	}

	stream->error = !stream->error; /* We couldn't read a character, so we have an error */
	return SO_EOF;
}

int so_fputc(int c, SO_FILE *stream) {
	if (writeRight(stream->opening_mode)) {
		if (stream->buffer_pointer == BUFF_SIZE) {
			if (so_fflush(stream) == SO_EOF) {
				stream->error = !stream->error;
				return SO_EOF;
			}
		}

		stream->buffer[stream->buffer_pointer++] = (char) c;
		stream->file_pointer++;
		stream->last_action = WRITE_OPERATION;
		return c;
	}

	stream->error = !stream->error; /* We couldn't write a character, so we have an error */
	return SO_EOF;
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
