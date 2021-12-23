#include <cstdio>

int N;
char buf[100];

int main() {
    fgets(buf, 100, stdin);

    scanf("%d", &N);
    if (N == 1) {
        printf("no it's impossible\n");
    } else {
        printf("yes\n");
        printf("lorem ipsum\n");
        printf(" dolor sit amet! \n");
    }
}
