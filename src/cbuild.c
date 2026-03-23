#include "cbuild.h"


int cbuild_run_command(const char *command) {
    return system(command);
}


int cbuild_runner_init(cbuild_runner *runner,const char * compiler)
{
    runner->compiler = compiler;
    return 0;
}

int cbuild_add_src(cbuild_runner *runner, const char *source_file)
{
    runner->source_files = source_file;
    return 0;
}

int cbuild_add_flags(cbuild_runner *runner, const char *flags)
{
    runner->flags = flags;
    return 0;
}

int cbuild_configure(cbuild_runner *runner)
{
    strcat(runner->output,runner->compiler);
    strcat(runner->output," ");
    strcat(runner->output, runner->source_files);
    strcat(runner->output," ");
    strcat(runner->output, runner->flags);
    strcat(runner->output, " -o");
    strcat(runner->output, " a.exe");
    return 0;
}

int cbuild_build(cbuild_runner * runner)
{
    system(runner->output);
    return 0;
}
