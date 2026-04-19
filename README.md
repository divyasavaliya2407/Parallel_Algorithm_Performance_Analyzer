# Parallel Algorithm Performance Analyzer


A modular C++ project designed to analyze and compare the performance of multiple algorithms using parallel computing techniques.

---

# Overview

This project evaluates the performance of different algorithms by implementing both **sequential and parallel approaches**, enabling comparison using key metrics such as execution time, speedup, and efficiency.

It serves as a practical demonstration of **parallel computing concepts** and performance optimization.

---

# Features

**Performance Analysis**

  * Measures execution time of algorithms

**Parallel Implementations**

  * Uses multithreading techniques for performance improvement

**Multiple Algorithms Supported**

  

**Modular Code Structure**

  * Clean separation using `include/` and `src/`

**Build Automation**

  * Simple build script for compilation

---

# Tech Stack

* **Language:** C++
* **Concepts:**

  * Multithreading
  * Parallel algorithms
  * Performance benchmarking
  * High-resolution timing (`chrono`)

---

# Project Structure

```
Parallel_Algorithm_Performance_Analyzer/
│
├── files/
│   └── large.txt                  
│
├── include/                       
│   ├── grayscale.h
│   ├── mandelbrot.h
│   ├── matrix.h
│   ├── merge_sort.h
│   ├── min.h
│   ├── monte_carlo.h
│   ├── num_search.h
│   ├── search_prime.h
│   ├── sum_array.h
│   └── wordcount.h
│
├── src/                           
│   ├── main.cpp
│   ├── grayscale.cpp
│   ├── mandelbrot.cpp
│   ├── matrix.cpp
│   ├── merge_sort.cpp
│   ├── min.cpp
│   ├── monte_carlo.cpp
│   ├── num_search.cpp
│   ├── search_prime.cpp
│   ├── sum_array.cpp
│   └── wordcount.cpp
│
├── build/                         
│   └── app
│
├── build.sh                       
├── .gitignore
└── README.md
```

---

# Build & Run

# Compile the Project

```bash
chmod +x build.sh
./build.sh
```

Or manually:

```bash
g++ -std=c++11 src/*.cpp -o build/app 
```

---

# Run the Application

```bash
./build/app
```

---

# Metrics Used

* **Execution Time**
  Time taken by each algorithm to execute

* **Speedup**
  Speedup = Sequential Time / Parallel Time

* **Efficiency**
  Efficiency = Speedup / Number of Threads

---

# Algorithms Implemented

| Category                     | Algorithm / File   | Description                                       | Parallelization Strategy                     |
| ---------------------------- | ------------------ | ------------------------------------------------- | -------------------------------------------- |
|  Numerical / Search        | `min.cpp`          | Finds the minimum element in an array             | Parallel reduction (array split into chunks) |
|  Numerical / Search        | `num_search.cpp`   | Searches for a target element in a dataset        | Parallel search across partitions            |
|  Numerical / Search        | `search_prime.cpp` | Finds prime numbers in a given range              | Range splitting across threads               |
|  Numerical / Search        | `sum_array.cpp`    | Computes sum of array elements                    | Parallel reduction                           |
|  Mathematical / Simulation | `monte_carlo.cpp`  | Estimates values (e.g., π) using random sampling  | Independent simulations per thread           |
|  Mathematical              | `matrix.cpp`       | Performs matrix operations (e.g., multiplication) | Row/column-wise parallel computation         |
|  Sorting                   | `merge_sort.cpp`   | Sorts data using divide-and-conquer               | Parallel recursive splitting & merging       |
|  Image Processing         | `grayscale.cpp`    | Converts image from RGB to grayscale              | Pixel-level parallel processing              |
|  Image Processing         | `mandelbrot.cpp`   | Generates Mandelbrot fractal image                | Pixel-wise independent computation           |
|  Text Processing           | `wordcount.cpp`    | Counts words in a text                            | Chunk-based parallel text processing         |

---

#What This Project Demonstrates

* Real-world use of **parallel algorithms**
* Performance gains from **multithreading**
* Trade-offs between sequential and parallel execution
* Scalability of different algorithm types

---

# Future Improvements

* Add configurable thread count via CLI
* Export results to CSV
* Add benchmarking across varying input sizes
* Integrate visualization tools

---



## Acknowledgement

AI tools were used for tasks such as code suggestions, debugging assistance, and documentation refinement. 


