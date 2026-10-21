
#include <stdio.h>

int permu(int n, int m) {
    int fn[10] = {0};
    int fm[10] = {0};

    while (n > 0) {
        fn[n % 10]++;
        n /= 10;
    }

    while (m > 0) {
        fn[m % 10]++;
        m /= 10;
    }

    for (int i = 0; i < 10; i++) {
        if (fn[i] != fm[i])
            return 0;
    }

    return 1; 
}

int main() {
    int n, m;

    scanf("%d", &n);
    scanf("%d", &m);

    if (permu(n, m)) {
        printf("SIM\n");
    } else {
        printf("NAO\n");
    }

    return 0;
}

