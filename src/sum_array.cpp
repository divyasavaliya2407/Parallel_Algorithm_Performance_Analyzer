#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

// --- Worker Function ---
// Calculates the sum of a specific chunk of the array
void parallelSumWorker(const vector<int>& arr, int startIdx, int endIdx, long long& partialResult) {
    long long localSum = 0;
    for (int i = startIdx; i < endIdx; ++i) {
        localSum += arr[i];
    }
    partialResult = localSum;
}

void run_sum() {
    // 1. Initialize a massive array (100 Million elements)
    const int N = 100000000;
    cout << "Allocating array of size " << N << "..." << endl;
    vector<int> data(N, 1); // Fill array with 1s. Total sum should be exactly N.

    // int numThreads = thread::hardware_concurrency(); // Get the number of concurrent threads supported by the hardware
    int numThreads = 8 ; // For demonstration, you can set this to the number of cores on your machine
    cout << "Using " << numThreads << " concurrent threads.\n" << endl;

    //               SERIAL EXECUTION
    auto startSerial = high_resolution_clock::now();
    
    long long serialSum = 0;
    for (int i = 0; i < N; ++i) {
        serialSum += data[i];
    }
    
    auto endSerial = high_resolution_clock::now();
    auto timeSerial = duration_cast<milliseconds>(endSerial - startSerial).count();

    
    //             PARALLEL EXECUTION

    auto startParallel = high_resolution_clock::now();

    vector<thread> threads;
    vector<long long> partialSums(numThreads, 0); // Store results from each thread
    int elementsPerThread = N / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startIdx = i * elementsPerThread;
        int endIdx = (i == numThreads - 1) ? N : (startIdx + elementsPerThread);  
        
        threads.push_back(thread(parallelSumWorker, ref(data), startIdx, endIdx, ref(partialSums[i])));
    }

    // The Synchronization Loop
    for (auto& t : threads) {
        t.join();
    }

    // Combine the partial results
    long long parallelSum = 0;

// Loop from index 0 to the size of the container
for (int i = 0; i < partialSums.size(); i++) {
    parallelSum += partialSums[i]; // Add the current element to the total sum
}

    auto endParallel = high_resolution_clock::now();
    auto timeParallel = duration_cast<milliseconds>(endParallel - startParallel).count();

    //             PERFORMANCE METRICS
  
    double speedup = static_cast<double>(timeSerial) / timeParallel;
    double efficiency = (speedup / numThreads) * 100;

    cout << "--- RESULTS ---" << endl;
    cout << "Serial Sum:   " << serialSum << " (Time: " << timeSerial << " ms)" << endl;
    cout << "Parallel Sum: " << parallelSum << " (Time: " << timeParallel << " ms)" << endl;
    
    cout << fixed << setprecision(2);
    cout << "\n--- METRICS ---" << endl;
    cout << "Speedup:      " << speedup << "x" << endl;
    cout << "Efficiency:   " << efficiency << "%" << endl;

    
}