#include <cstdio>

int A, B;

int main() {
    scanf("%d %d", &A, &B);

    if (B == 5) {
        printf("%d\n", A + B);
    } else if (B == 3) {
        printf("%d\n", A + B + 1);
    } else if (B == 4) {
        return 1 / (B - 4);
    } else {
        while (true);
    }
}
