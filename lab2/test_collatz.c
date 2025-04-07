#include <stdio.h>
#include "collatz.h"

#define MAX_ITER 1000

int main() {
    int input = 27;  // Przykładowa liczba wejściowa
    int steps[MAX_ITER];

    int num_steps = test_collatz_convergence(input, MAX_ITER, steps);

    if (num_steps == 0) {
        printf("Collatz conjecture did not converge within %d iterations\n", MAX_ITER);
    } else {
        printf("Collatz sequence for %d:\n", input);
        for (int i = 0; i < num_steps; i++) {
            printf("%d ", steps[i]);
        }
        printf("\n");
    }

    return 0;
}
