#include <cstdio>

int T;
int A, B;

int main() {
    scanf("%d", &T);

    for (int tc = 0; tc < T; tc++) {
        scanf("%d %d", &A, &B);
        printf("Case \"$%d\\\": %d\n", tc+1, A + B);
    }
}
