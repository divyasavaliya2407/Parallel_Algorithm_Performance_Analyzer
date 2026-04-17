#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Define a simple Pixel structure to hold RGB values
struct Pixel
{
    unsigned char r, g, b;
};

// Parallel Grayscaling function
void GrayscalingFunction(const vector<Pixel> &input, vector<unsigned char> &output, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        output[i] = static_cast<unsigned char>(
            0.299 * input[i].r + 0.587 * input[i].g + 0.114 * input[i].b); // Luminosity formula: 0.299R + 0.587G + 0.114B
    }
}

void run_grayscale()
{
    const int width = 1920;
    const int height = 1080;
    const int totalPixels = width * height;

    // 1. Create a sample image (for testing, we can use a random color)
    vector<Pixel> image(totalPixels, {255, 100, 50}); // Reddish-orange image
    vector<unsigned char> resultSerial(totalPixels);
    vector<unsigned char> resultParallel(totalPixels);

    // SERIAL VERSION
    auto startS = high_resolution_clock::now();
    GrayscalingFunction(image, resultSerial, 0, totalPixels);
    auto endS = high_resolution_clock::now();
    auto timeSerial = duration_cast<microseconds>(endS - startS).count();

    // PARALLEL VERSION
    // int numThreads = thread::hardware_concurrency();     // Uncomment this line to use the number of hardware threads available on your machine
    int numThreads = 8; // For demonstration, you can set this to the number of cores on your machine
    vector<thread> threads;
    int pixelsPerThread = totalPixels / numThreads;

    auto startP = high_resolution_clock::now();
    for (int i = 0; i < numThreads; ++i)
    {
        int start = i * pixelsPerThread;
        int end = (i == numThreads - 1) ? totalPixels : (start + pixelsPerThread);

        threads.push_back(thread(GrayscalingFunction, ref(image), ref(resultParallel), start, end));
    }

    for (auto &t : threads)
        t.join();
    auto endP = high_resolution_clock::now();
    auto timeParallel = duration_cast<microseconds>(endP - startP).count();

    // 2. Output Performance
    cout << "Image Size: " << width << "x" << height << " (" << totalPixels << " pixels)" << endl;
    cout << "Serial Time:   " << timeSerial << " us" << endl;
    cout << "Parallel Time: " << timeParallel << " us (" << numThreads << " threads)" << endl;
    cout << "Speedup:       " << (double)timeSerial / timeParallel << "x" << endl;

    
}