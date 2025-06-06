#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

#define DEBUG 1

void add_NewEntry(const char*);

bool assert_OpenDir(const char *path, DIR **);

bool assert_Database(const char*);

void get_PidDetail_pidname(const char*);

void get_PidDetail_cmd(const char*);

bool has_NewEntry(const char*, const char *);

void init_Database();

void track_DirectoryChanges(const char *);

