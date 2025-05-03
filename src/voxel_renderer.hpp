#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include "noise.hpp"

class VoxelRenderer {
private:
    int gridWidth;
    int gridHeight;
    int gridDepth;
    float voxelSize;
    std::vector<float> voxelData;
    PerlinNoise noiseGenerator;
    uint64_t currentSeed;
    
    // Display lists for OpenGL
    GLuint displayListBase;
    bool displayListsGenerated;
    
    // Camera properties
    float cameraX, cameraY, cameraZ;
    float rotX, rotY;
    
    // Threshold for rendering voxels
    float densityThreshold;
    
public:
    VoxelRenderer(int width = 32, int height = 32, int depth = 32, float size = 0.1f);
    ~VoxelRenderer();
    
    // Generate new terrain with a specific or random seed
    void generateTerrain(uint64_t seed = 0);
    
    // Regenerate with current seed
    void regenerateTerrain();
    
    // Get current seed value
    uint64_t getSeed() const;
    
    // Set density threshold for rendering
    void setDensityThreshold(float threshold);
    
    // Camera controls
    void moveCamera(float dx, float dy, float dz);
    void rotateCamera(float dx, float dy);
    
    // Render the voxel grid
    void render();
    
private:
    // Generate OpenGL display lists for faster rendering
    void generateDisplayLists();
    
    // Delete display lists to free memory
    void deleteDisplayLists();
    
    // Draw a single voxel cube
    void drawVoxel(float x, float y, float z, float size, float density);
    
    // Check if a voxel should be drawn based on noise value
    bool isVoxelVisible(int x, int y, int z) const;
}; 