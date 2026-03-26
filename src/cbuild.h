#ifndef CBUILD_H
#define CBUILD_H


#if defined (_WIN32)
#include <windows.h>
#endif

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> // Required for string manipulation

#define GCC "gcc"
#define MSVC "cl"
#define CLANG "clang"
#define DEBUGFLAG "-g"


typedef struct {
    char file[500];
} cbuild_file;




typedef struct {
    const char *compiler;
    char output[500];
    int success;
    pthread_t worker[10];
    pthread_mutex_t mux;
    pthread_cond_t cond;
    cbuild_file files[100];
    int count;
    int stop;

} cbuild_runner;


int cbuild_run_command(const char *command);

void cbuild_self_compile(cbuild_runner *runner,const char* filepath);
int cbuild_runner_init(cbuild_runner *runner,const char * compiler);
int cbuild_append(cbuild_runner *runner,...);
int cbuild_append_source(cbuild_runner *runner,...);
int cbuild_configure(cbuild_runner *runner);
int cbuild_build(cbuild_runner *runner);
int cbuild_rename_file(const char *old_path, const char *new_path);
uint64_t hasFileBeenUpdated(const char *filepath);
uint64_t SerializeFileTime(const FILETIME* ft);


void* cbuild_run_worker(void* arg);
void cbuild_queue_add_task(cbuild_runner *q, const char* filename);






#endif // CBUILD_H

#ifdef CBUILD_INCLUDE_IMPL
#include <stdarg.h>
#include <assert.h>

void cbuild_self_compile(cbuild_runner *runner,const char* filepath) {
#ifdef _WIN32
    //uint64_t current = readTimeStamp("blob.bin");
    uint64_t current = hasFileBeenUpdated("output.exe");
    uint64_t updated = hasFileBeenUpdated(filepath);
    if(current < updated) {
        const char cmd[500]="";
        cbuild_rename_file("output.exe", "output.old.exe");
        strcat(cmd,runner->compiler);
        strcat(cmd," ");
        strcat(cmd, " src/**.c");
        strcat(cmd, " ");
        strcat(cmd, " -o output.exe");
        cbuild_run_command(cmd);
        printf("output.exe -> output.old.exe\n");
        cbuild_run_command("output.exe");
        exit(1);
    }
#endif
}

int cbuild_run_command(const char *command) {
    return system(command);
}


int cbuild_rename_file(const char *old_path, const char *new_path) {
#ifdef _WIN32
    if (!MoveFileEx(old_path, new_path, MOVEFILE_REPLACE_EXISTING)) {
        printf("could not rename %s to %s: %s\n", old_path, new_path);
        return 0;
    }
    return 1;
#endif
}

int cbuild_runner_init(cbuild_runner *runner,const char * compiler)
{
    runner->success=0;
    runner->compiler = compiler;
    strcpy(runner->output, runner->compiler);
    strcat(runner->output, " ");
    runner->count = 0;

    return 0;
}

int cbuild_append(cbuild_runner *runner,...)
{
    va_list ap;
    va_start(ap, runner);

    const char *arg;
    while ((arg = va_arg(ap, const char *)) != NULL) {
        if (arg[strlen(arg)-1]=='c' && arg[strlen(arg)-2] == '.')
        {
            char builder[100] = "";
            char output_obj[50]="";
            strcat(output_obj, arg);
            strcat(output_obj, ".o");
            uint64_t fileTime = hasFileBeenUpdated(arg);
            uint64_t binaryTime=hasFileBeenUpdated(output_obj);

            if(fileTime>binaryTime)
            {


                runner->success=1;
                //strcat(builder, "gcc -c ");
                //strcat(builder, arg);
                //strcat(builder, " -o ");
                //strcat(builder, output_obj);
                //cbuild_run_command(builder);

                cbuild_queue_add_task(runner,arg);

            }

            printf("%s\n", builder);
            strcat(runner->output, output_obj);
            strcat(runner->output, " ");

        }
        else{
            strcat(runner->output,arg);
            strcat(runner->output, " ");
        }

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

    pthread_mutex_init(&runner->mux, NULL);
    pthread_cond_init(&runner->cond, NULL);
    for (int i = 0; i < 10; i++) {
        pthread_create(&runner->worker[i], NULL, cbuild_run_worker, runner);
    }
    if(runner->success==0)
    {
        for (int i = 0; i < 10; i++) {
            pthread_join(runner->worker[i], NULL);
        }
        printf("[CBUILD]:Nothing to Build No file Updated");
    }
    else{
        for (int i = 0; i < 10; i++) {
            pthread_join(runner->worker[i], NULL);
        }
        printf("[CBUILD]:%s\n", runner->output);
        cbuild_run_command(runner->output);
    }

    return 0;
}


#if defined(_WIN32)
uint64_t SerializeFileTime(const FILETIME* ft) {
    return ((uint64_t)ft->dwHighDateTime << 32) | (uint64_t)ft->dwLowDateTime;
}
#endif
uint64_t hasFileBeenUpdated(const char *filepath) {
#if defined(_WIN32)
    WIN32_FILE_ATTRIBUTE_DATA info;

    if (!GetFileAttributesExA(filepath, GetFileExInfoStandard, &info)) {
        printf("Could not open file: %lu\n", GetLastError());
        return 1;
    }

    FILETIME lastTime = info.ftLastWriteTime;

    // Compare with last known write time
    return SerializeFileTime(&lastTime);
#endif
}


// Worker function
void* cbuild_run_worker(void* arg) {
    cbuild_runner *runner = (cbuild_runner*)arg;
    while (1) {

        pthread_mutex_lock(&runner->mux);

        // Wait if no tasks
        while (runner->count == 0) {
            pthread_mutex_unlock(&runner->mux);
            return NULL;
        }

        // Take task
        cbuild_file file = runner->files[--runner->count];

        pthread_mutex_unlock(&runner->mux);

        // Process task
        char builder[100] = "";
        char output_obj[50]="";
        strcat(output_obj, file.file);
        strcat(output_obj, ".o");
        strcat(builder, "gcc -c ");
        strcat(builder, file.file);
        strcat(builder, " -o ");
        strcat(builder, output_obj);
        printf("[CBUILD]:%s\n",builder);
        cbuild_run_command(builder);
    }
    return NULL;
}

// Add task
void cbuild_queue_add_task(cbuild_runner *q, const char* filename) {
    snprintf(q->files[q->count].file,
             sizeof(q->files[q->count].file),
             "%s", filename);
    q->count++;
}
#endif
