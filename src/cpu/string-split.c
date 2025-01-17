#include <string.h>
#include <stdlib.h>

char** split_string(const char* str_in, const char * delimiter) {
    char* val;
    char** retval;
    int count = 0;
    char* token;

    char* temp = strdup(str_in);
    token = strtok(temp, delimiter);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }
    free(temp);
    retval = malloc(count * sizeof(char *));
    val = strdup(str_in);

    token = strtok(val, delimiter);
    count = 0;
    while (token != NULL) {
        retval[count] = strdup(token);
        count++;
        token = strtok(NULL, delimiter);
    }
    return retval;
}

int count_num_entries(const char** result) {
    int i = 0;
    while (result[i] != NULL)
        i++;
    return i;
}

void free_split_result(const char** result) {
    int i = 0;
    while (result[i] != NULL)
        free((void *)result[i++]);
    free(result);
}