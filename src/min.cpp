#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;
int findmax(const vector<int>& data, int start, int end, int& result){
    int localMin = data[start];
    for(int i=start+1;i<end;i++){
        if(data[i]<localMin){
            localMin = data[i];
        }
    }
    result = localMin;
    return localMin;
}
void run_min(){
    const int SIZE = 50000000; // 50 million elements
    cout<<"generating "<<SIZE<<" random numbers..."<<endl;
    vector<int> data(SIZE);
    random_device rd;
    mt19937 gen(rd()); 
    uniform_real_distribution<double> dist(0.0, 1000.0);
    for(int i=0;i<SIZE;i++){
        data[i] = dist(gen);
    }
    cout<<"starting serial min search..."<<endl;
    auto startS = high_resolution_clock::now(); 
    int minVal = data[0];
    for(int i=1;i<SIZE;i++){
        if(data[i]<minVal){
            minVal = data[i];
        }
    }
    auto endS = high_resolution_clock::now();
    auto timeSerial = duration_cast<milliseconds>(endS - startS).count();
    cout<<"minimum value: "<<minVal<<" in "<<timeSerial<<" ms"<<endl;
    auto startP = high_resolution_clock::now();
    // int numThreads = thread::hardware_concurrency();
    int numThreads = 8; // you can set this to the number of cores on your machine
    vector<thread> threads;
    int chunksize=SIZE/numThreads;  
    vector<int> minResults(numThreads,0);
    cout<<"starting parallel min search with "<<numThreads<<" threads..."<<endl;
    for(int i=0;i<numThreads;i++){
        int start =i*chunksize;
        int end = (i==numThreads-1) ? SIZE : start+chunksize;
        threads.push_back(thread(findmax, cref(data), start, end, ref(minResults[i])));
    }
    for(int i=0;i<numThreads;i++){
        threads[i].join();
    }
    int parallelMin = minResults[0];
    for(int i=1;i<numThreads;i++){
        if(minResults[i]<parallelMin){
            parallelMin = minResults[i];
        }
    }
    auto endP = high_resolution_clock::now();
    auto timeParallel = duration_cast<milliseconds>(endP - startP).count();
    cout<<"minimum value: "<<parallelMin<<" in "<<timeParallel<<" ms"<<endl;
    double speedup = (double)timeSerial / timeParallel;
    cout<<"speedup: "<<speedup<<endl;   
    double efficiency = (speedup / numThreads) * 100.0;
    cout<<"efficiency: "<<efficiency<<"%"<<endl;
    
    
}