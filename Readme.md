A Package Manager ,Some Libs and Build Tool for c

```c
#define CBUILD_INCLUDE_IMPL
#include "cbuild.h"

int main(int argc, char *argv[]) {

    cbuild_runner runner;
    cbuild_runner_init(&runner, GCC);
    cbuild_self_compile(&runner, __FILE__);
    cbuild_append(&runner, "test.c", NULL);
    cbuild_append(&runner, "x.c", NULL);
    cbuild_append(&runner, "-o","test.exe",NULL);
    cbuild_build(&runner);
    return 0;
}
