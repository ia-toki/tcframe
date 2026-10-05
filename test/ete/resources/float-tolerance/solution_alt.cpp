#include <cstdio>

// Off by at most 5e-8, within the 1e-6 tolerance.
int main() {
    int A, B;
    scanf("%d %d", &A, &B);
    printf("%.7f\n", (double) A / B);
    return 0;
}
