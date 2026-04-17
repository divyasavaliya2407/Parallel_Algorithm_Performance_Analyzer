#include <iostream>
#include "../include/matrix.h"
#include "../include/monte_carlo.h"
#include "../include/sum_array.h"
#include "../include/search_prime.h"
#include "../include/num_search.h"
#include "../include/min.h"
#include "../include/grayscale.h"
using namespace std;

int main(){
    int choice;
    cout << "1. Matrix Multiplication\n";
    cout << "2. Array Sum\n";
    cout << "3. Prime Search\n";
    cout << "4. Monte Carlo Pi\n";
    cout << "5. Min\n";
    cout << "6. Num_Search\n";
    cout << "7. Gray_Scale\n";
    cout << "Enter choice: ";

    cin >> choice;
    switch(choice){
        case 1: run_matrix(); break;
        case 2: run_sum(); break;
        case 3: run_prime(); break;
        case 4: run_montecarlo(); break;
        case 5: run_min(); break;
        case 6: run_Search_Numbers(); break;
        case 7: run_gray_scale(); break;
        default: cout << "Invalid choice\n";
    }
}
    
