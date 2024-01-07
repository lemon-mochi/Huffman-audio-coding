#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

//this function opens a file dialog box and returns the string indicating the 
//location of the selected file
//works on ubuntu linux I don't know about other distros or os
char* open_file() {
    // Run the zenity command to choose a file
    FILE* pipe = popen("zenity --file-selection", "r");
    if (!pipe) {
        fprintf(stderr, "Error running zenity\n");
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    char* filePath = NULL;

    // Read the selected file path from the pipe
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Remove trailing newline character
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Allocate memory for filePath and copy the buffer
        filePath = (char*)malloc(sizeof(char) * (len + 1));  // +1 for the null terminator
        if (filePath != NULL) {
            strcpy(filePath, buffer);
        }
    }

    // Close the pipe
    pclose(pipe);

    // Check if a file path was selected
    if (filePath == NULL || filePath[0] == '\0') {
        fprintf(stderr, "No file selected\n");
        free(filePath);
        return NULL;
    }

    // Now 'filePath' contains the selected file path
    return filePath;
}
