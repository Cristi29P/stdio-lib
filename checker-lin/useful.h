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
#define READ_OPERATION 10
#define WRITE_OPERATION 20

int get_flags(const char *mode);

int readRight(const char *mode);


#endif //USEFUL_H
