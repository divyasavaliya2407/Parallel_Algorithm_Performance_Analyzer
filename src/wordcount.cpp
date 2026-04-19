#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>


// Counts words in a specific slice of the text
void countWords(const std::vector<std::string>& lines, int start, int end, int& result) {
    int count = 0;
    
    for (int i = start; i < end; ++i) {
        bool inWord = false;
        for (char c : lines[i]) {
            if (c == ' '||c == '\t'||c == '\n'||c == '\r') {
                inWord = false;
            } else if (!inWord) {
                inWord = true;
                count++;
            }
        }
    } 
    result = count;
}


// Single thread counter
int countWordsSequential(const std::vector<std::string>& lines) {
    int totalCount = 0;
    countWords(lines, 0, lines.size(), totalCount);
    return totalCount;
}


// Multiple threads counter
int countWordsParallel(const std::vector<std::string>& lines) {
    int num = std::thread::hardware_concurrency(); 
    if (num == 0) num = 4; // if hardware_concurrency fails in a device then this saves the code.

    std::vector<std::thread> threads;
    std::vector<int> results(num, 0); 
    
    int linesPerThread = lines.size() / num;

    for (int i = 0; i < num; ++i) {
        int start = i * linesPerThread;
        int end = (i == num - 1) ? lines.size() : start + linesPerThread;  
        threads.push_back(std::thread(countWords, std::ref(lines), start, end, std::ref(results[i])));
    }

    int total = 0;
    for (int i = 0; i < num; ++i) {
        threads[i].join(); 
        total += results[i]; 
    }

    return total;
}

void run_wordcount() {
    
    std::string filename = "files/large.txt"; 
    std::ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        
    }

    std::cout << "Loading file " << std::endl;
    std::vector<std::string> allLines;
    std::string currentLine;
    
    while (std::getline(inputFile, currentLine)) {
        allLines.push_back(currentLine);
    }
    inputFile.close();
    std::cout << "File loaded. Total lines: " << allLines.size() << "\n\n";

    
    std::cout << "Starting Sequential Count" << std::endl;
    auto startSeq = std::chrono::high_resolution_clock::now();
    
    int seqWords = countWordsSequential(allLines);
    
    auto endSeq = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationSeq = endSeq - startSeq;
    
    std::cout << "Sequential Words: " << seqWords << std::endl;
    std::cout << "Sequential Time:  " << durationSeq.count() << " ms\n\n";

    
    std::cout << "--- Starting Parallel Count ---" << std::endl;
    std::cout << "Using " << std::thread::hardware_concurrency() << " threads." << std::endl;
    
    auto startPar = std::chrono::high_resolution_clock::now();
    
    int parWords = countWordsParallel(allLines);
    
    auto endPar = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationPar = endPar - startPar;
    
    std::cout << "Parallel Words: " << parWords << std::endl;
    std::cout << "Parallel Time:  " << durationPar.count() << " ms\n\n";

    
    if (seqWords == parWords) {
        std::cout << "SUCCESS: Both methods produced the exact same word count." << std::endl;
        double speedup = durationSeq.count() / durationPar.count();
        std::cout << "Speedup Factor: " << speedup << "x faster using multiple threads." << std::endl;
    } else {
        std::cout << "ERROR: Word counts do not match!" << std::endl;
    }

    
}