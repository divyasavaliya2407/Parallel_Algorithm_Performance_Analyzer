#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
const int N = 1000; 
using namespace std;


void multiply_serial(const vector<int> &A,const vector<int> &B, vector<int> &c_series){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            int sum = 0;
            for(int k=0;k<N;k++){
                sum+=A[i*N+k]*B[k*N+j];
            }
            c_series[i*N+j]=sum;
        }
    }
}


void multiply_parallel(const vector<int> &A,const vector<int> &B,vector<int> &c_parallel,int start_row,int end_row){
    for(int i=start_row;i<end_row;i++){
        for(int j=0;j<N;j++){
            int sum = 0;
            for(int k=0;k<N;k++){
                sum+= A[i*N+k]* B[k*N+j];
            }
            c_parallel[i*N+j]=sum;
        }
    }
}
// void display(const vector<int> &c_series){
//     for(int i=0;i<N;i++){
//         for(int j=0;j<N;j++){
//             cout<<c_series[i*N+j]<<" ";
//         }
//         cout<<endl;
//     }
// }
void run_matrix(){
    // first we will create matrices `
    
    vector<int> A(N*N , 1) ,B(N*N , 2),c_series(N*N , 0),c_parallel(N*N , 0);
    auto start = chrono::high_resolution_clock::now();
    cout<<"Performing Serial Multiplication..."<<endl;
    multiply_serial(A,B,c_series);
    // display(c_series);
    auto end = chrono::high_resolution_clock::now();
    auto serial_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken by serial method: " << serial_duration.count() << " milliseconds" << endl;


    // lets do parallel multiplication
    int num_threads = thread::hardware_concurrency();  // for maximum threads supported by hardware
    vector<thread> threads;
    int rows_per_thread = N / num_threads;
    start = chrono::high_resolution_clock::now();
    cout<<"Performing Parallel Multiplication with "<<num_threads<<" threads..."<<endl;
    for(int i =0;i<num_threads;i++){
        int start_row = i*rows_per_thread;
        int end_row;
        if(i==num_threads-1){
            end_row=N;
        }else{
            end_row = start_row + rows_per_thread;
        }
        threads.push_back(thread(multiply_parallel,ref(A),ref(B),ref(c_parallel),start_row,end_row));
    }
    cout<<"Waiting for threads to finish..."<<endl;
    // now we will join the threads
    for(auto &t:threads){
        t.join();
    }
    end = chrono::high_resolution_clock::now();
    cout<<"Parallel Multiplication Completed."<<endl;
    auto parallel_duration = chrono::duration_cast<chrono::milliseconds>(end - start); 

    cout << "Time taken by parallel method: " << parallel_duration.count() << " milliseconds" << endl;

    
}