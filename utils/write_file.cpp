#include <iostream>
#include "rsa_utils.h"


unsigned char* read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror(filename);
        exit(1);
    }

    // Move the file pointer to the end to determine the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);  // Reset the file pointer to the beginning

    // Allocate memory to store the file content
    unsigned char *buffer = (unsigned char*)malloc(file_size);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file content into the buffer
    if (fread(buffer, 1, file_size, file) != file_size) {
        perror("Failed to read file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;  // Return the buffer containing the file data
}
