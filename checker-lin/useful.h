//
// Created by cristian on 30.03.2022.
//

#ifndef USEFUL_H
#define USEFUL_H

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 4096
#define PATH_LENGTH 1024
#define MODE_LENGTH 5
#define INVALID_FLAGS -1

int get_flags(const char *mode);


#endif //USEFUL_H
