#!/bin/bash

clang++ -o collatz_caching_set collatz_caching_set.cpp -lgmp -L/opt/homebrew/lib -I/opt/homebrew/include -stdlib=libc++
clang++ -o collatz_caching_vector collatz_caching_vector.cpp -lgmp -L/opt/homebrew/lib -I/opt/homebrew/include -stdlib=libc++
clang++ -o collatz_caching_hybrid collatz_caching_hybrid.cpp -lgmp -L/opt/homebrew/lib -I/opt/homebrew/include -stdlib=libc++
