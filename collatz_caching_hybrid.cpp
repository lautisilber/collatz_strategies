#include <gmpxx.h>
#include <string>
#include <unordered_set>
#include <iostream>

#include "mem_usage.h"

// ---- Hash mpz_class -> 64-bit signature (lightweight) ----
uint64_t hash_mpz_64(const mpz_class &x) {
    // Export raw bytes
    size_t size = (mpz_sizeinbase(x.get_mpz_t(), 2) + 7) / 8;
    std::vector<unsigned char> buf(size);
    mpz_export(buf.data(), nullptr, 1, 1, 0, 0, x.get_mpz_t());

    // FNV-1a hash
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : buf) {
        h ^= c;
        h *= 1099511628211ULL;
    }
    return h;
}

std::unordered_set<uint64_t> tested_small;
std::vector<mpz_class> tested_large;

bool large_cached_contains(mpz_class const& x) {
    return std::binary_search(tested_large.begin(), tested_large.end(), x);
}

void sort_large_cache() {
    std::sort(tested_large.begin(), tested_large.end());
    tested_large.erase(std::unique(tested_large.begin(), tested_large.end()), tested_large.end());
}

bool can_cache_small(const mpz_class &n) {
    static const size_t SMALL_LIMIT_BITS = 1024;
    size_t bits = mpz_sizeinbase(n.get_mpz_t(), 2);

    return bits <= SMALL_LIMIT_BITS;
}

bool cached_contains(const mpz_class &n) {
    if (can_cache_small(n)) {
        uint64_t sig = hash_mpz_64(n);
        return tested_small.find(sig) != tested_small.end();
    } else {
        return large_cached_contains(n);
    }
}

void add_to_cache(const mpz_class &n) {
    if (can_cache_small(n)) {
        uint64_t sig = hash_mpz_64(n);
        tested_small.insert(sig);
    } else {
        tested_large.push_back(n);
        if (tested_large.size() % 100000 == 0)
            sort_large_cache();
    }
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

int main() {
    mpz_class n = 1;
    tested_small.reserve(1 << 20); // reserve 1M

    for (;;++n) {
        if (is_collatz(n)) {
            add_to_cache(n);
            if (n % 1000000 == 0)
                gmp_printf("%Zd is collatz\n", n.get_mpz_t());
        } else {
            gmp_printf("%Zd is NOT collatz!\n", n.get_mpz_t());
            break;
        }
    }
}
