#include "stdio.h"
// #include "solver.h"
#define MAX 11

int main() {
    char line[MAX];
    fgets(line, MAX, stdin);
    printf(line, "%s");
    return 0;
}
