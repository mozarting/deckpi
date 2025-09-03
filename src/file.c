#include "../include/file.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: cannot open file '%s'\n", file_path);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "ERROR: failed to seek file '%s'\n", file_path);
        fclose(fp);
        return NULL;
    }
    long filesize = ftell(fp);
    if (filesize < 0) {
        fprintf(stderr, "ERROR: failed to get file size of '%s'\n", file_path);
        fclose(fp);
        return NULL;
    }
    rewind(fp);

    char *buffer = malloc(filesize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, filesize, fp);
    if (read_size != (size_t)filesize) {
        fprintf(stderr, "ERROR: failed to read entire file '%s'\n", file_path);
        free(buffer);
        fclose(fp);
        return NULL;
    }

    buffer[filesize] = '\0';
    fclose(fp);
    return buffer;
}
