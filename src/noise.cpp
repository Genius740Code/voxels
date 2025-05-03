#include "noise.hpp"
#include <iostream>
#include <chrono>
#include <string>
#include <limits>

// Generate voxel terrain using fractal noise
void generateFractalTerrain(std::vector<float>& voxelData, int width, int height, int depth, 
                          const PerlinNoise& noise, float scale) {
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Calculate 3D coordinate with scaling
                float fx = x * scale;
                float fy = y * scale;
                float fz = z * scale;
                
                // Get fractal noise value for this coordinate
                float noiseValue = noise.fractal(fx, fy, fz, 6, 2.0, 0.5);
                
                // Map noise from [-1,1] to [0,1]
                noiseValue = (noiseValue + 1.0f) * 0.5f;
                
                // Store in voxel data array
                size_t index = static_cast<size_t>(x + y * width + z * width * height);
                if (index < voxelData.size()) {
                    voxelData[index] = noiseValue;
                }
            }
        }
    }
}

// Utility function to get a large random seed (up to 10 billion)
uint64_t getRandomSeed() {
    // Use current time as basis for seed
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()).count();
    
    // Combine with another source of randomness
    std::random_device rd;
    uint64_t randomBits = static_cast<uint64_t>(rd()) << 32 | rd();
    
    // Mix the values to get a seed in the range up to 10 billion
    uint64_t seed = (nanos ^ randomBits) % 10000000000ULL;
    return seed;
} 