#include <cstdio>

// Off by up to 5e-4, outside the 1e-6 tolerance.
int main() {
    int A, B;
    scanf("%d %d", &A, &B);
    printf("%.3f\n", (double) A / B);
    return 0;
}
