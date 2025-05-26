#include <stdio.h>

int main() {
    while (1) {
        double x = 1.0;
        for (int i = 0; i < 1000000; ++i) {
            x *= 1.000001;
        }
    }
    return 0;
}
