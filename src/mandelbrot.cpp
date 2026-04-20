#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::chrono;


struct MandelPixel {
    double r, g, b;
};


int computeMandel(double cr, double ci, int max_iter) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    while (zr * zr + zi * zi <= 4.0 && iter < max_iter) {
        double temp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = temp;
        iter++;
    }

    return iter;
}


void renderMandelSlice(int startY, int endY, int width, int height,
                       int max_iter, vector<MandelPixel>& framebuffer) {

    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t index = static_cast<size_t>(y) * width + x;

            double cr = (double)x / width * 3.0 - 2.0;
            double ci = (double)y / height * 3.0 - 1.5;

            int iter = computeMandel(cr, ci, max_iter);
            double t = (double)iter / max_iter;

            framebuffer[index] = { t, t * t, sqrt(t) };
        }
    }
}


void saveMandelPPM(const string& filename,
                   const vector<MandelPixel>& framebuffer,
                   int width, int height) {

    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file!\n";
        return;
    }

    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& p : framebuffer) {
        int r = (int)(p.r * 255);
        int g = (int)(p.g * 255);
        int b = (int)(p.b * 255);
        file << r << " " << g << " " << b << "\n";
    }
}


void run_mandelbrot() {
    int width = 800;
    int height = 600;
    int max_iter = 500;

    vector<MandelPixel> framebuffer(width * height);

    cout << "--- Mandelbrot Benchmark ---\n";

    
    auto startSerial = high_resolution_clock::now();
    renderMandelSlice(0, height, width, height, max_iter, framebuffer);
    auto endSerial = high_resolution_clock::now();

    auto serialTime = duration_cast<milliseconds>(endSerial - startSerial);
    cout << "Serial Time: " << serialTime.count() << " ms\n";

    
    int numThreads = thread::hardware_concurrency();
    if (numThreads <= 0) numThreads = 4;

    numThreads = min(numThreads, height);

    int rowsPerThread = (height + numThreads - 1) / numThreads;

    vector<thread> threads;

    auto startParallel = high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = min(startY + rowsPerThread, height);

        if (startY >= height) break;

        threads.emplace_back(renderMandelSlice,
                             startY, endY,
                             width, height,
                             max_iter,
                             ref(framebuffer));
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    auto endParallel = high_resolution_clock::now();
    auto parallelTime = duration_cast<milliseconds>(endParallel - startParallel);

    cout << "Parallel Time (" << numThreads << " threads): " << parallelTime.count() << " ms\n";

    // SPEEDUP
    double speedup = 0.0;
    if (parallelTime.count() > 0) {
        speedup = (double)serialTime.count() / parallelTime.count();
    }

    cout << "Speedup: " << speedup << "x\n";

    // SAVE
    saveMandelPPM("./build/mandelbrot.ppm", framebuffer, width, height);
    cout << "Saved as mandelbrot.ppm\n";
}
