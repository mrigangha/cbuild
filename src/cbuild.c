#include "cbuild.h"

#include <stdarg.h>
#include <assert.h>


void cbuild_self_compile(cbuild_runner *runner,const char* filepath) {
    uint64_t current = readTimeStamp("blob.bin");
    uint64_t updated = hasFileBeenUpdated(filepath);
    if(current != updated) {
        const char cmd[50];
        strcat(cmd,runner->compiler);
        strcat(cmd," ");
        strcat(cmd, " src/**.c");
        strcat(cmd, " ");
        strcat(cmd, " -o output1.exe");
        cbuild_run_command(cmd);
        writeTimeStamp("blob.bin", updated);
        printf("Self-compilation failed\n");
        exit(1);
    }
}

int cbuild_run_command(const char *command) {
    return system(command);
}


int cbuild_runner_init(cbuild_runner *runner,const char * compiler)
{
    runner->compiler = compiler;
    strcat(runner->output, compiler);
    strcat(runner->output, " ");
    return 0;
}

int cbuild_append(cbuild_runner *runner,...)
{
    va_list ap;
    va_start(ap, runner);

    const char *arg;
    while ((arg = va_arg(ap, const char *)) != NULL) {
        strcat(runner->output, arg);
        strcat(runner->output, " ");
    }

    va_end(ap);
    return 0;
}

int cbuild_configure(cbuild_runner *runner)
{
    strcat(runner->output, " -o");
    strcat(runner->output, " a.exe");
    return 0;
}

int cbuild_build(cbuild_runner * runner)
{
    printf("%s\n", runner->output);
    system(runner->output);
    return 0;
}


uint64_t SerializeFileTime(const FILETIME* ft) {
    return ((uint64_t)ft->dwHighDateTime << 32) | (uint64_t)ft->dwLowDateTime;
}

int hasFileBeenUpdated(const char *filepath) {
    WIN32_FILE_ATTRIBUTE_DATA info;

    if (!GetFileAttributesExA(filepath, GetFileExInfoStandard, &info)) {
        printf("Could not open file: %lu\n", GetLastError());
        return 1;
    }

    FILETIME lastTime = info.ftLastWriteTime;

    // Compare with last known write time
    return SerializeFileTime(&lastTime);
}


void writeTimeStamp(const char *filepath, uint64_t original_number) {
    FILE *file_ptr;

        // --- Writing the integer to a binary file ---
        // Open file in binary write mode ("wb")
    file_ptr = fopen(filepath, "wb");
    if (file_ptr == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Write the integer's raw binary data to the file
    // fwrite(pointer_to_data, size_of_each_item, number_of_items, file_pointer)
    fwrite(&original_number, sizeof(uint64_t), 1, file_ptr);


    // Close the file after writing
    fclose(file_ptr);
}

uint64_t readTimeStamp(const char *filepath) {
    uint64_t read_number = 0;
    FILE* file_ptr = fopen(filepath, "rb");
    if (file_ptr == NULL) {
        return 0;  // ← 0 means "no cached value", not an error code
    }
    fread(&read_number, sizeof(uint64_t), 1, file_ptr);
    fclose(file_ptr);
    return read_number;
}
