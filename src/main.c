#include "cbuild.h"

int main() {
    cbuild_runner runner;
    cbuild_runner_init(&runner, "gcc");
    cbuild_add_src(&runner, "test.c");
    cbuild_add_flags(&runner, "-g");
    cbuild_configure(&runner);
    cbuild_build(&runner);
    return 0;
}
