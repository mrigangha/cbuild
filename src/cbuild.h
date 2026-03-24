#ifndef CBUILD_H
#define CBUILD_H



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // Required for string manipulation

#define GCC "gcc"
#define CLANG "CLANG"
#define DEBUGFLAG "-g"

typedef struct {
    const char *compiler;
    char output[500];
    int success;
} cbuild_runner;


int cbuild_run_command(const char *command);

void cbuild_self_compile(cbuild_runner *runner,const char* filepath);
int cbuild_runner_init(cbuild_runner *runner,const char * compiler);
int cbuild_append(cbuild_runner *runner,...);
int cbuild_configure(cbuild_runner *runner);
int cbuild_build(cbuild_runner *runner);
int cbuild_rename_file(const char *old_path, const char *new_path);
uint64_t readTimeStamp(const char *filepath);
void writeTimeStamp(const char *filepath, uint64_t original_number);
int hasFileBeenUpdated(const char *filepath);
uint64_t SerializeFileTime(const FILETIME* ft);
#endif // CBUILD_H
