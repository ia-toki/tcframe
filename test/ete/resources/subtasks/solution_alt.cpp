#include <cstdio>

int A, B;

int main() {
    scanf("%d %d", &A, &B);
    if (A > 10) {
        printf("%d\n", A + B + 1);
    } else {
        printf("%d\n", A + B);
    }
}
