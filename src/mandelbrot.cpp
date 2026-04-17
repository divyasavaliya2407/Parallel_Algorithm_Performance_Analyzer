#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <cmath>


using namespace std;
using namespace std::chrono;
struct Pixel {
    double r, g, b;
};


// Mandelbrot computation for one pixel
int mandelbrot(double cr, double ci, int max_iter) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    while (zr*zr + zi*zi <= 4.0 && iter < max_iter) {
        double temp = zr*zr - zi*zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = temp;
        iter++;
    }

    return iter;
}


// Render slice (used by threads)

void renderSlice(int startY, int endY, int width, int height,
                 int max_iter, vector<Pixel>& framebuffer) {

    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {

            // Map pixel to complex plane (-2 to 1, -1.5 to 1.5)
            double cr = (double)x / width * 3.0 - 2.0;
            double ci = (double)y / height * 3.0 - 1.5;

            int iter = mandelbrot(cr, ci, max_iter);

            double t = (double)iter / max_iter;

            // Simple coloring
            framebuffer[y * width + x] = {
                t,
                t * t,
                sqrt(t)
            };
        }
    }
}


// Save image

void savePPM(const string& filename,
             const vector<Pixel>& framebuffer,
             int width, int height) {

    ofstream file(filename);
    file << "P3\n" << width << " " << height << "\n255\n";

    for (const auto& p : framebuffer) {
        int r = p.r* 255;
        int g = p.g* 255;
        int b = p.b* 255;

        file << r << " " << g << " " << b << " ";
    }
}


void run_mandelbrot() {
    int width = 800;
    int height = 600;
    int max_iter = 500;

    vector<Pixel> framebuffer(width * height);

    cout << "--- Mandelbrot Benchmark ---\n";

   
    // SERIAL
    
    auto startSerial = high_resolution_clock::now();

    renderSlice(0, height, width, height, max_iter, framebuffer);

    auto endSerial = high_resolution_clock::now();

    auto serialTime = duration_cast<milliseconds>(endSerial - startSerial);

    cout << "Serial Time: " << serialTime.count() << " ms\n";

   
    // PARALLEL
    
    int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    vector<thread> threads;

    int rowsPerThread = height / numThreads;

    auto startParallel = high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? height : startY + rowsPerThread;

        threads.emplace_back(renderSlice, startY, endY, width, height, max_iter, ref(framebuffer));
    }

    for (auto& t : threads) {
        t.join();
    }

    auto endParallel = high_resolution_clock::now();

    auto parallelTime = duration_cast<milliseconds>(endParallel - startParallel);

    cout << "Parallel Time (" << numThreads << " threads): "
              << parallelTime.count() << " ms\n";


    // SPEEDUP
  
    double speedup = (double)serialTime.count() / parallelTime.count();

    cout << "Speedup: " << speedup << "x\n";

   
    // SAVE OUTPUT

    savePPM("mandelbrot.ppm", framebuffer, width, height);

    cout << "Saved as mandelbrot.ppm\n";

    
}