# A Build system for c..

Building C/C++ project must require only a c compiler.

## Features.
1. Use C to build a executable build files.
2. No dependency only c compiler.
3. Auto builds itself.
4. Concurrent builds using pthread pool
## Hot to use..
1. Include the cbuild.h header
2. use #define CBUILD_INCLUDE_IMPL
3. And use the cbuild_runner struct to create object and use it.
### Planned feature
1. Parsing dependency and includes and autobuild everthing.

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
