#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    VERTEX,
    FRAGMENT
} SHADER_TYPE;

// Utility function to concatenate two strings
// I'm tired of seeking normal solution
char* concatStr(char* str1, char* str2) {
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);

    // Allocates memory for new string
    char* result = (char*) calloc(str1_len + str2_len + 1, sizeof(char));

    for(int i = 0; i < str1_len; i++) {
        result[i] = str1[i];
    }
    for(int i = 0; i < str1_len; i++) {
        result[i + str1_len] = str2[i];
    }

    return result;
}

// Returns shader string from file
char* getShader(SHADER_TYPE shader_type) {
    FILE* fptr;
    char* fileLocation = "./src/shaders/";
    char* result = (char*) malloc(0);

    // Chooses which file to use 
    switch(shader_type) {
        case VERTEX: 
            fileLocation = concatStr(fileLocation, "vertex.glsl");
            break;
        case FRAGMENT:
            fileLocation = concatStr(fileLocation, "fragment.glsl");
            break;
    }

    fptr = fopen(fileLocation, "r");

    // Writes all characters from file to result string
    char ch;
    int result_len = 0;
    while((ch = fgetc(fptr)) != EOF) {
        result = (char*) realloc(result, ++result_len * sizeof(char));
        result[result_len-1] = ch;
    }
    result[result_len] = '\0';

    // Returns result string :3
    return result;
}