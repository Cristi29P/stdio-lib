/* @Copyright Paris Cristian-Tanase / Operating Systems 2022 */
#include "so_stdio.h"
#include "useful.h"

struct _so_file {
	pid_t pid; /* Pid associated with the child process */
	int fd; /* The associated file descriptor */
	int file_pointer; /* Cursor position inside file */
	int buffer_pointer; /* Cursor position inside buffer */
	int last_bytes; /* Last number of bytes added to the buffer*/
	int last_action; /* Last I/O operation on the buffer */
	int error; /* Flag set if error occurred during I/O tasks */
	int eof_reached; /* Flag set if eof reached during I/O tasks */
	char opening_mode[MODE_LENGTH]; /* File read/write/append permissions */
	char buffer[BUFF_SIZE]; /* The associated file buffer used for I/O tasks */
};

SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	SO_FILE *f = NULL;
	int flags = get_flags(mode);

	if (flags == INVALID_FLAGS)
		return NULL;

	int fd = open(pathname, flags, 0644);

	if (fd == SO_EOF)
		return NULL;

	f = calloc(1, sizeof(SO_FILE));
	if (!f)
		return NULL;

	f->pid = -1; /* Default file opening mode has no process attached */
	f->fd = fd;
	f->file_pointer = 0;
	f->buffer_pointer = 0;
	f->last_bytes = 0;
	f->last_action = 0; /* No action performed so far */
	f->error = 0;
	f->eof_reached = 0;
	strncpy(f->opening_mode, mode, MODE_LENGTH);
	memset(f->buffer, '\0', BUFF_SIZE); /* Make sure the buffer is empty */

	return f;
}

int so_fclose(SO_FILE *stream)
{
	/* The buffer needs to be flushed if last I/O task was to write */
	if (stream->last_action == WRITE_OPERATION) {
		if (so_fflush(stream)) {
			stream->error = 1;
			close(stream->fd);
			free(stream);
			return SO_EOF;
		}
	}

	int rt = close(stream->fd);

	free(stream);
	return rt;
}

int so_fileno(SO_FILE *stream)
{
	return stream->fd;
}

int so_fflush(SO_FILE *stream)
{
	if (stream->last_action == WRITE_OPERATION) {
		int bytes_written = 0;
		/* Write the entire buffer to the file */
		/* Loop implementation taken from lab 2 solutions (xwrite function) */
		while (bytes_written < stream->buffer_pointer) {
			int bytes_actually = (int)write(stream->fd, stream->buffer + bytes_written,
										   stream->buffer_pointer - bytes_written);

			if (bytes_actually < 0) {
				stream->error = 1;
				return SO_EOF;
			}

			bytes_written += bytes_actually;
		}

		memset(stream->buffer, '\0', BUFF_SIZE);
		stream->last_bytes = 0;
		stream->buffer_pointer = 0;
		return 0;
	}

	return SO_EOF;
}

int so_fseek(SO_FILE *stream, long offset, int whence)
{
	/* Invalidate the buffer if the last I/O task was read */
	if (stream->last_action == READ_OPERATION) {
		memset(stream->buffer, '\0', BUFF_SIZE);
		stream->buffer_pointer = 0;
		stream->last_bytes = 0;
	}
	/* Flush the buffer if the last I/O task was write */
	if (stream->last_action == WRITE_OPERATION) {
		if (so_fflush(stream)) {
			stream->error = 1;
			return SO_EOF;
		}
	}

	off_t ret = lseek(stream->fd, offset, whence);

	if (ret == SO_EOF)
		return SO_EOF;

	stream->file_pointer = (int)ret; /* Set new file pointer location */

	return 0;
}

long so_ftell(SO_FILE *stream)
{
	return stream->file_pointer;
}

size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int bytes_read = 0;

	while (bytes_read < (int) (size * nmemb)) {
		char ch = (char) so_fgetc(stream);

		if (so_feof(stream) || so_ferror(stream))
			break;

		((char *) ptr)[bytes_read++] = ch;
	}

	return (bytes_read / size);
}

size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int bytes_written = 0;

	while (bytes_written < (int) (size * nmemb)) {
		so_fputc((int) ((char *) ptr)[bytes_written++], stream);
		if (so_ferror(stream))
			break;
	}
	return (bytes_written / size);
}


int so_fgetc(SO_FILE *stream)
{
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

int so_fputc(int c, SO_FILE *stream)
{
	if (writeRight(stream->opening_mode)) {
		if (stream->buffer_pointer == BUFF_SIZE) {
			if (so_fflush(stream) == SO_EOF) {
				stream->error = 1;
				return SO_EOF;
			}
		}

		stream->buffer[stream->buffer_pointer++] = (char) c;
		stream->file_pointer++;
		stream->last_action = WRITE_OPERATION;
		return c;
	}

	stream->error = 1; /* We couldn't write a character, so we have an error */
	return SO_EOF;
}

int so_feof(SO_FILE *stream)
{
	return stream->eof_reached;
}

int so_ferror(SO_FILE *stream)
{
	return stream->error;
}

SO_FILE *custom_file(pid_t pid, int file_descriptor, const char *mode)
{
	SO_FILE *f = NULL;

	if (get_flags(mode) == INVALID_FLAGS)
		return NULL;

	f = calloc(1, sizeof(SO_FILE));
	if (!f)
		return NULL;

	f->pid = pid;
	f->fd = file_descriptor;
	f->file_pointer = 0;
	f->buffer_pointer = 0;
	f->last_bytes = 0;
	strncpy(f->opening_mode, mode, MODE_LENGTH);
	memset(f->buffer, '\0', BUFF_SIZE);

	return f;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	SO_FILE *f = NULL;
	pid_t pid;
	int file_des[2], fd_file;

	if (pipe(file_des))
		return NULL;

	pid = fork();
	switch (pid) {
	case -1:
		/* Fork failed */
		close(file_des[PIPE_READ]);
		close(file_des[PIPE_WRITE]);
		return NULL;
	case 0:
		/* Child process */
		if (readRight(type)) {
			if (close(file_des[PIPE_READ])) /* Close unused input pipe end */
				return NULL;
			/* Redirect output of command to pipe output end in child process */
			dup2(file_des[PIPE_WRITE], STDOUT_FILENO);
			if (close(file_des[PIPE_WRITE])) /* Close the old fd */
				return NULL;
		} else {
			if (close(file_des[PIPE_WRITE])) /* Close unused output pipe end */
				return NULL;
			/* Redirect input of command to pipe input end in child process */
			dup2(file_des[PIPE_READ], STDIN_FILENO);
			if (close(file_des[PIPE_READ])) /* Close the old fd */
				return NULL;
		}
		char *const argvec[] = {"sh", "-c", (char *const)command, NULL};

		execvp("sh", argvec);
		exit(EXIT_FAILURE);
	default:
		/* Parent process */
		if (readRight(type)) {
			if (close(file_des[PIPE_WRITE]))
				return NULL;

			fd_file = file_des[PIPE_READ];
		} else {
			if (close(file_des[PIPE_READ]))
				return NULL;

			fd_file = file_des[PIPE_WRITE];
		}
		break;
	}
	f = custom_file(pid, fd_file, type);

	return f;
}

int so_pclose(SO_FILE *stream)
{
	pid_t pid = stream->pid;
	int status;

	if (pid != -1) { /* This means the file was opened via so_popen */
		if (so_fclose(stream))
			return SO_EOF;

		if (waitpid(pid, &status, 0) < 0)
			return -1;

		return status;
	}

	/* We cannot call so_pclose on a file not opened with so_popen */
	return -1;
}
