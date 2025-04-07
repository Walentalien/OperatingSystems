#include <stdio.h>

int collatz_conjecture(int input) {
    if (input % 2 == 0) {
        return input / 2;
    } else {
        return 3 * input + 1;
    }
}


int test_collatz_convergence(int input, int max_iter, int *steps) {
    int count = 0;
    int current = input;

    while (current != 1 && count < max_iter) {
        steps[count] = current;
        current = collatz_conjecture(current);
        count++;
    }


    if (current != 1) {
        return 0;
    }

    // Jeśli zbieżność do 1 została osiągnięta, zwróć liczbę kroków
    steps[count] = current;  // Dodaj ostatni krok (1)
    return count + 1;  // Zwróć liczbę wykonanych kroków
}
