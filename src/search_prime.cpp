#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace std::chrono;

// --- 1. The Math Function ---
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

// --- 2. The Parallel Worker Function ---
void countPrimesChunk(int startNum, int endNum, int& partialCount) {
    int count = 0;
    for (int i = startNum; i < endNum; ++i) {
        if (isPrime(i)) {
            count++;
        }
    }
    partialCount = count;
}

void run_prime() {
    const int N = 5000000;
    int numThreads = 8;

    cout << "Finding primes up to " << N << " using " << numThreads << " threads...\n" << endl;

    // SERIAL
    auto startS = high_resolution_clock::now();

    int serialCount = 0;
    for (int i = 0; i < N; ++i) {
        if (isPrime(i)) serialCount++;
    }

    auto endS = high_resolution_clock::now();
    auto timeSerial = duration_cast<milliseconds>(endS - startS).count();

    // PARALLEL
    auto startP = high_resolution_clock::now();

    vector<thread> threads;
    vector<int> partialCounts(numThreads, 0);
    int rangePerThread = N / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startNum = i * rangePerThread;
        int endNum = (i == numThreads - 1) ? N : (startNum + rangePerThread);

        threads.push_back(thread(countPrimesChunk, startNum, endNum, ref(partialCounts[i])));
    }

    for (auto& t : threads) {
        t.join();
    }

    int parallelCount = 0;
    for (int i = 0; i < partialCounts.size(); i++) {
        parallelCount += partialCounts[i];
    }

    auto endP = high_resolution_clock::now();
    auto timeParallel = duration_cast<milliseconds>(endP - startP).count();

    double speedup = static_cast<double>(timeSerial) / timeParallel;

    cout << "Serial Count:   " << serialCount << " (Time: " << timeSerial << " ms)" << endl;
    cout << "Parallel Count: " << parallelCount << " (Time: " << timeParallel << " ms)" << endl;
    cout << fixed << setprecision(2);
    cout << "Speedup:        " << speedup << "x" << endl;
    cout << "Efficiency:     " << (speedup / numThreads) * 100 << "%" << endl;
}


