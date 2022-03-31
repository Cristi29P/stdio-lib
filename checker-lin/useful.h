#ifndef USEFUL_H
#define USEFUL_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 4096
#define MODE_LENGTH 5
#define INVALID_FLAGS -1
#define NO_ACTION 10
#define READ_OPERATION 11
#define WRITE_OPERATION 12

int get_flags(const char *mode);

int readRight(const char *mode);

int writeRight(const char *mode);


#endif //USEFUL_H
