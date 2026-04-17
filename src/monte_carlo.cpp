#include <iostream>
#include <random>
#include <thread> 
#include <chrono>
#include <vector>
#include <numeric>
#include <iomanip>
  
double time_serial;
double time_parallel;

// --- Worker Function ---
long long monte_carlo_pi_worker(long long samples) {
    // Thread-local random generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    long long inside_circle = 0;
    for (long long i = 0; i < samples; ++i) {
        double x = dis(gen);
        double y = dis(gen);

        if (x * x + y * y <= 1.0) {
            inside_circle++;
        }
    }
    return inside_circle;
}

// --- Serial Execution ---
void run_serial(long long total_samples) {
    auto start_time = std::chrono::high_resolution_clock::now();

    long long inside = monte_carlo_pi_worker(total_samples);
    double pi_estimate = (static_cast<double>(inside) / total_samples) * 4.0;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "#Serial Execution \n";
    std::cout << "Pi Estimate: " << std::setprecision(7) << pi_estimate << "\n";
    std::cout << "Time Taken:  " << elapsed.count() << " seconds\n\n";
    time_serial = elapsed.count();
}

// --- Parallel Execution ---
void run_parallel(long long total_samples, unsigned int num_cores) {
    auto start_time = std::chrono::high_resolution_clock::now();

    long long samples_per_core = total_samples / num_cores;
    long long remainder = total_samples % num_cores;

    std::vector<std::thread> threads;
    std::vector<long long> results(num_cores, 0);

    for (unsigned int i = 0; i < num_cores; ++i) {
        long long samples_for_this_thread = samples_per_core;
        if (i == 0) {
            samples_for_this_thread += remainder;
        }

        threads.emplace_back([i, samples_for_this_thread, &results]() {   
            results[i] = monte_carlo_pi_worker(samples_for_this_thread);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    long long total_inside = std::accumulate(results.begin(), results.end(), 0LL);

    double pi_estimate = (static_cast<double>(total_inside) / total_samples) * 4.0;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "#Parallel Execution (" << num_cores << " cores)\n";
    std::cout << "Pi Estimate: " << std::setprecision(7) << pi_estimate << "\n";
    std::cout << "Time Taken:  " << elapsed.count() << " seconds\n\n";

    time_parallel = elapsed.count();
}

void run_montecarlo() {
    const long long TOTAL_SAMPLES = 50'000'000;

    unsigned int num_cores = std::thread::hardware_concurrency();
    if (num_cores == 0) num_cores = 4;

    std::cout << "Estimating Pi using Monte Carlo simulation (" 
              << TOTAL_SAMPLES << " samples)\n\n";

    run_serial(TOTAL_SAMPLES);
    run_parallel(TOTAL_SAMPLES, num_cores);
    double speedup = time_serial/time_parallel;
    std::cout << "Speedup: " << speedup<<"\n";    
    double efficiency = (speedup / num_cores) * 100.0;
    std::cout<<"Efficiency: "<<efficiency<<"%"<<"\n";
    
}