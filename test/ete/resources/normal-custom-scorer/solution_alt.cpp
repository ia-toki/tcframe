#include <cstdio>

int A, B;

int main() {
    scanf("%d %d", &A, &B);
    if (B == 3) {
        // WA
        printf("%.2lf\n", A + B + 0.2);
    } else {
        // AC
        printf("%.2lf\n", A + B + 0.02);
    }
}
