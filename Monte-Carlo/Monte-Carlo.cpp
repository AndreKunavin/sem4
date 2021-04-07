#include <iostream>
#include <future>
#include <thread>
#include <fstream>
#include "Vector2.hpp"

#define UN_REP 100000
std::vector <std::size_t> counter;

void pi() {
    std::size_t N_in = 0;
    for (std::size_t i = 0; i < UN_REP ; i++) {
        auto v = Vector2::rand(); 
        if (v.len() < 1) {
            N_in++;
        }
    }
    counter.push_back(N_in);
}

int main() {
    std::size_t open_threads = std::thread::hardware_concurrency();
    std::cout << "OPEN THREADS = " << open_threads << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::future<void>> f;
    for (std::size_t i = 0; i < open_threads; i++) {
        f.push_back(std::async(pi));
    }
    for(auto& result : f) result.get();
    std::size_t N_in = 0;
    for (std::size_t i = 0; i < open_threads; i++) {
        N_in += counter[i];
    }
    float S = 4 * (float)N_in / (float)(open_threads * UN_REP);
    auto diff = std::chrono::high_resolution_clock::now() - start;
    printf("%.3f\n", S);
    std::cout << diff.count() / 10e8 << " seconds" << std::endl;
}