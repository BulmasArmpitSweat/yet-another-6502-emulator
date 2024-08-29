#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    gettimeofday(&time, NULL);
    printf("%ld", time.tv_sec);
    return 0;
}