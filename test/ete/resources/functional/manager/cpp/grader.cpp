#include <cstdio>

#include "decoder.h"
#include "encoder.h"

int main() {
    int n;
    scanf("%d", &n);
    printf("%d\n", decode(encode(n)));
    return 0;
}
