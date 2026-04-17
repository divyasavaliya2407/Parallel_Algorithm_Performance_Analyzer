#include<iostream>
#include<cmath>
#include <random>
#include<vector>
#include <thread>
#include <chrono>
using namespace std;
void merge(vector<int>&vec,int low,int mid,int high)
{
    vector<int>temp(high-low+1);
    int left=low,right =mid+1 ,k=0;
    while(left<=mid && right <=high)
    {
        if(vec[left]<=vec[right])
        {
            temp[k]=vec[left];
            left++;k++;
        }
        else
        {
            temp[k]=vec[right];
            right++;k++;
        }
    }
    while(left<=mid)
    {
         temp[k]=vec[left];
         left++; k++;
    }
    while(right <=high)
    {
         temp[k]=vec[right];
            right++; k++;
    }

    for(int i=low; i<=high; i++)
    {
        vec[i]=temp[i-low];
    }
    return ;
}


void divide_parallel(vector<int>&vec,int low,int high,int depth,int max_depth)
{
    if(low>=high)
    return ;
    int mid= low+(high-low)/2;
    if(depth<max_depth && high-low>=10000)
    {
        thread t(divide_parallel,ref(vec),low , mid,depth+1,max_depth); // creating new threads
        divide_parallel(vec,mid+1,high,depth+1,max_depth); 
        t.join();
    }
    else{
    divide_parallel(vec,low,mid,depth+1,max_depth);
    divide_parallel(vec,mid+1,high,depth+1,max_depth);
    }
    merge(vec,low,mid,high);
}
void divide_serial(vector<int>&vec,int low,int high)
{
    if(low>=high)
    return ;
    int mid= low+(high-low)/2;
     divide_serial(vec,low,mid);
    divide_serial(vec,mid+1,high);
    merge(vec,low,mid,high);
}

void run_mergesort()
{
   int n = 5000000;
    vector<int> arr(n); 

    random_device rd;
    mt19937 gen(rd());  // GENERATE RANDOM NUMBERS
    uniform_int_distribution<> dist(1, n); // TO CHOOSE RANGE

    for (int i = 0; i < n; i++) {
        arr[i]=(dist(gen));
    }
   
    int num_thread;
    cout<<"How many threads do you want to use"<<endl;
    cin>>num_thread;

    if(num_thread==0)
     num_thread=1;

    int hard_concurrency= std:: thread :: hardware_concurrency(); // CALCULATES THREADS AVAILABLE IN OUR SYSTEM
    if(hard_concurrency==0)
    hard_concurrency = 2;
    num_thread = min(num_thread, hard_concurrency);
    int max_depth =(int)log2(num_thread);  // SO  THAT WE STOP CREATING THREADS THAN GIVEN AS INPUT BY USER

    vector<int>vec=arr;

    // PARALLEL COMPUTING
    auto start1 = chrono::high_resolution_clock::now();
    divide_parallel(arr,0,arr.size()-1,0,max_depth);

    auto end1= chrono::high_resolution_clock::now();  
    
    auto time1= chrono::duration_cast<chrono::milliseconds>(end1-start1);  // PARALLEL TIME TAKEN
    cout<<"Parallely time taken: "<<time1.count()<<" ms\n";

    // SERIAL COMPUTING
    auto start2 = chrono::high_resolution_clock::now();
      divide_serial(vec,0,vec.size()-1);
    auto end2= chrono::high_resolution_clock::now();  
    auto time2= chrono::duration_cast<chrono::milliseconds>(end2-start2);  // SERIAL TIME TAKEN
     cout<<"Serial time taken: "<<time2.count()<<" ms   ";

  
}
