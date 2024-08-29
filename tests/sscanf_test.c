#include <stdio.h>

int main() {
    char _unused_result_1[100], _unused_result_2[100];
    char* test[2] = {"Hello_There:", "Hello_There;"};
    int result = sscanf(test[0], "%[^:]:", _unused_result_1) + sscanf(test[1], "%[^:]:", _unused_result_2);
    printf("%d\n", result);
    // printf("%d\n", sscanf(test[0], "%s:", _unused_result_1));
    // printf("%d\n", sscanf(test[1], "%s:", _unused_result_2));
    return 0;
}