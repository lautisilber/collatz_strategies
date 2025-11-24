#include <gmpxx.h>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>

#include "mem_usage.h"

struct MpzHash {
    std::size_t operator()(mpz_class const& x) const noexcept {
        // Hash the binary representation of the number
        size_t size = mpz_sizeinbase(x.get_mpz_t(), 256);
        std::vector<unsigned char> buf(size);
        mpz_export(
            buf.data(),     // writable buffer
            nullptr,
            1,              // order
            1,              // size of each word in bytes
            0,              // use native endianness (SAFE for size=1)
            0,              // nail bits
            x.get_mpz_t()
        );
        return std::hash<std::string_view>{}(
            std::string_view(reinterpret_cast<char*>(buf.data()), buf.size())
        );
    }
};

struct MpzEqual {
    bool operator()(mpz_class const& a, mpz_class const& b) const noexcept {
        return a == b;
    }
};

std::unordered_set<mpz_class, MpzHash, MpzEqual> tested_ints;

bool is_collatz(mpz_class n) {
    while (n != 1) {
        if (tested_ints.find(n) != tested_ints.end()) { // this value has already been tested
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
    tested_ints.reserve(1 << 20); // reserve 1M

    for (;;++n) {
        if (is_collatz(n)) {
            tested_ints.insert(n);
            if (n % 1000000 == 0)
                gmp_printf("%Zd is collatz\n", n.get_mpz_t());
        } else {
            gmp_printf("%Zd is NOT collatz!\n", n.get_mpz_t());
            break;
        }
    }
}
