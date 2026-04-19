#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
// bubble sort function
void bubblesort(std::vector<int>& arr,int start,int end){
    for(int i=start;i<end-1;i++){
        for(int j=start;j<end-(i-start)-1;j++){
            if(arr[j]>arr[j+1]){
                int temp =arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
}
// merging function for different sorted parts
void merge1(std::vector<int>& arr,int start,int end1 ,int end){
    int s1=start,s=start;
    int n=end1;
    const int N=end-start+1;
    int s2=n+1;
    std::vector<int> v(N);
    int i=0;
    while(s1<=n&&s2<=end){
        v[i++]=(arr[s1]>arr[s2])?arr[s2++]:arr[s1++];
    }
    while(s1<=n){
        v[i++]=arr[s1++];
    }
    while(s2<=end){
        v[i++]=arr[s2++];
    }
    for(int i=0;i<N;i++){
        arr[s+i]=v[i];
    }
}

void run_bubblesort(){
    const int n=10000;//no. of elements
    const int nt=8;// no. of threads
    std::vector<int> arr(n);//declaring a array
    std::vector<std::thread> threads;
    //generating a random permutated array of of n elemnt of range 1 to n
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    std::srand(std::time(0));

    for (int i = n-1; i > 0; i--) {
        int j = std::rand() % (i + 1);

        // swap
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    std::vector<int> arr2(arr);// copying the previous array into anothere for serial execution
    auto start1 = std::chrono::high_resolution_clock::now();
    bubblesort(arr2,0,n);//serial execution
    auto end1 = std::chrono::high_resolution_clock::now();
    auto time1= std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1);

    auto start2 = std::chrono::high_resolution_clock::now();
    //starting parallel execution
    for(int i=0;i<nt;i++){
        int start=i*n/nt;
        int end=i*n/nt+n/nt;
        threads.emplace_back(bubblesort,std::ref(arr),start,end);
    }
    for(int i=0;i<nt;i++){
        threads[i].join();
    }
    int n2=0;
    int n1=n/nt;
    for(int i=1;i<nt;i++){
        n2=i*n1-1;
        int end=n1+n2;
        merge1(arr,0,n2,end);
    }
    //parallel execution ends
    auto end2 = std::chrono::high_resolution_clock::now();
    auto time2= std::chrono::duration_cast<std::chrono::milliseconds>(end2-start2);
    std::cout<<"serial time taken = " << time1.count()<<"\n";
    std::cout<<"thread used : "<<nt<<"\n";
    std::cout<<"parallel time taken = " << time2.count()<<"\n";
    double efficiency=(double)time1.count()/time2.count();
    double percentage=efficiency/nt*100;
    std::cout << std::fixed << std::setprecision(2);
    std::cout<<"speed up = "<<efficiency<<'\n';
    std::cout<<"efficiency = "<<percentage<<"\n";
    
}