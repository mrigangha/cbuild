#ifndef CBUILD_H
#define CBUILD_H

#include <stdio.h>
#include <stdlib.h> // Required for the system function
#include <string.h> // Required for string manipulation



typedef struct {
    const char *compiler;
    const char *source_files;
    const char *flags;
    const char output[500];
    int success;
} cbuild_runner;


int cbuild_run_command(const char *command);


int cbuild_runner_init(cbuild_runner *runner,const char * compiler);
int cbuild_add_src(cbuild_runner *runner, const char *source_file);
int cbuild_add_flags(cbuild_runner *runner, const char *flags);
int cbuild_configure(cbuild_runner *runner);
int cbuild_build(cbuild_runner * runner);

#endif // CBUILD_H
