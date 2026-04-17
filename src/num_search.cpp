#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;
std::mutex mtx;       
int finalIndex = -1;  

int serialSearch(const vector<int>& arr, int target){
    for(int i=0;i<(int)arr.size();i++){
        if(arr[i] == target){
                return i;
        } 
    }
    return -1;
}
void parallelsearch(const vector<int>&vec,int target,int start,int end){
    for(int i=start;i<end;i++){
        if(finalIndex!=-1) return;
        if(vec[i]==target){
            mtx.lock();
            if(finalIndex==-1){
                finalIndex=i;
            }
            mtx.unlock();
        }
        
        
    }
    return;
}
void run_Search_Numbers(){
    const int SIZE = 50000000; // 50 million elements
    int target;
    cout<<"generating "<<SIZE<<" random numbers..."<<endl;
    vector<int> data(SIZE);
    random_device rd;
    mt19937 gen(rd()); 
    uniform_real_distribution<double> dist(0.0, 1000.0);
    for(int i=0;i<SIZE;i++){
        data[i] = dist(gen);
    }
    target = 1001;
    data[SIZE-100] = target;
    cout<<"starting serial search..."<<endl;
    auto startS = high_resolution_clock::now();
    int sresult;
    sresult = serialSearch(data, target);
    auto endS = high_resolution_clock::now();   
    auto timeSerial = duration_cast<milliseconds>(endS - startS).count(); // in milliseconds
    cout<<"found at index: "<<sresult<<" in "<<timeSerial<<" ms"<<endl;
    // int numThreads = thread::hardware_concurrency();
    int numThreads =8;
    vector<thread> threads;
    int chunksize=SIZE/numThreads;
    cout<<"number of threads: "<<numThreads<<endl;
    cout<<"starting parallel search..."<<endl;
    auto startP = high_resolution_clock::now();
    for(int i=0;i<numThreads;i++){
        int start = i*chunksize;
        int end;
        if(i==numThreads-1){
            end = SIZE;
        } else {
            end = start + chunksize; 
        }
        threads.push_back(thread(parallelsearch, ref(data), target, start, end));
    }
    for(auto& t: threads){
        t.join();
    }
    auto endP = high_resolution_clock::now();
    auto timeParallel = duration_cast<milliseconds>(endP - startP).count(); // in milliseconds
    cout<<"found at index: "<<finalIndex<<" in "<<timeParallel<<" ms"<<endl;
    double speedup = (double)timeSerial / timeParallel;
    cout<<"speedup: "<<speedup<<endl;
    double efficiency = (speedup / numThreads) * 100.0;
    cout<<"efficiency: "<<efficiency<<"%"<<endl;

    
}