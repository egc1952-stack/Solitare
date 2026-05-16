#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include "sqlite3.h"


int main() {
    ThreadPool pool; // uses hardware_concurrency()

    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i) {
        results.push_back(
            pool.enqueue([i] {
                // CPU work here
                int sum = 0;
                for (int k = 0; k < 1'000'000; ++k) sum += (i + k);
                return sum;
            })
        );
    }

    for (auto &f : results) {
        std::cout << f.get() << "\n";
    }
}
