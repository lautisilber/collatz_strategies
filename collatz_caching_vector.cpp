#include <gmpxx.h>
#include <string>
#include <vector>
#include <iostream>

#include "mem_usage.h"

std::vector<mpz_class> tested_ints;

bool cached_contains(mpz_class const& x) {
    return std::binary_search(tested_ints.begin(), tested_ints.end(), x);
}

bool is_collatz(mpz_class n) {
    while (n != 1) {
        if (cached_contains(n)) { // this value has already been tested_ints
            break;
        } else if ((n & 1) == 0) {    // even
            n >>= 1;                  // n /= 2
        } else {                      // odd
            n = 3*n + 1;
        }
    }
    return true;
}

void sort_cache() {
    std::sort(tested_ints.begin(), tested_ints.end());
    tested_ints.erase(std::unique(tested_ints.begin(), tested_ints.end()), tested_ints.end());
}

int main() {
    mpz_class n = 1;
    tested_ints.reserve(1 << 20); // reserve 1M

    std::ofstream("log_vector_caching.csv")


    for (;;++n) {
        if (is_collatz(n)) {
            tested_ints.push_back(n);
            if (tested_ints.size() % 100000 == 0)
                sort_cache();
            if (n % 1000000 == 0)
                gmp_printf("%Zd is collatz\n", n.get_mpz_t());
        } else {
            gmp_printf("%Zd is NOT collatz!\n", n.get_mpz_t());
            break;
        }
    }
}
