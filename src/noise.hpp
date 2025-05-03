#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

class PerlinNoise {
private:
    std::vector<int> permutation;
    uint64_t seed;

    // Fade function for smoother interpolation
    double fade(double t) const {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // Linear interpolation
    double lerp(double t, double a, double b) const {
        return a + t * (b - a);
    }

    // Gradient function
    double grad(int hash, double x, double y, double z) const {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

public:
    // Constructor with extended seed range (up to 10 billion)
    PerlinNoise(uint64_t seed = 0) {
        this->seed = seed;
        reseed(seed);
    }

    // Reseed the noise generator with a new seed
    void reseed(uint64_t newSeed) {
        seed = newSeed;
        
        // Initialize permutation table with values 0-255
        permutation.resize(256);
        std::iota(permutation.begin(), permutation.end(), 0);
        
        // Use modern random engine with 64-bit seed for better distribution
        std::mt19937_64 engine(seed);
        
        // Shuffle the permutation table using the seeded engine
        std::shuffle(permutation.begin(), permutation.end(), engine);
        
        // Double the permutation table to avoid overflow
        permutation.resize(512);
        for (int i = 0; i < 256; i++) {
            permutation[i + 256] = permutation[i];
        }
    }

    // Get the current seed
    uint64_t getSeed() const {
        return seed;
    }

    // Classic Perlin noise 3D (return value between -1 and 1)
    double noise(double x, double y, double z) const {
        // Find unit cube that contains the point
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        // Find relative x, y, z of point in cube
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        // Compute fade curves for each of x, y, z
        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        // Hash coordinates of the 8 cube corners
        int A = permutation[X] + Y;
        int AA = permutation[A] + Z;
        int AB = permutation[A + 1] + Z;
        int B = permutation[X + 1] + Y;
        int BA = permutation[B] + Z;
        int BB = permutation[B + 1] + Z;

        // Add blended results from 8 corners of cube
        return lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z),
                                      grad(permutation[BA], x - 1, y, z)),
                              lerp(u, grad(permutation[AB], x, y - 1, z),
                                   grad(permutation[BB], x - 1, y - 1, z))),
                      lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1),
                                   grad(permutation[BA + 1], x - 1, y, z - 1)),
                           lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1),
                                grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
    }

    // Fractal Brownian Motion (fBm) - fractal noise summation
    double fractal(double x, double y, double z, int octaves = 6, double lacunarity = 2.0, double persistence = 0.5) const {
        double total = 0.0;
        double frequency = 1.0;
        double amplitude = 1.0;
        double maxValue = 0.0;

        for (int i = 0; i < octaves; i++) {
            total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        // Normalize the result to a range of [-1, 1]
        return total / maxValue;
    }
}; 