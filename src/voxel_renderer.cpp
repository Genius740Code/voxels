#include "voxel_renderer.hpp"
#include <iostream>

// Function declarations
extern uint64_t getRandomSeed();
extern void generateFractalTerrain(std::vector<float>& voxelData, int width, int height, int depth, 
                                 const PerlinNoise& noise, float scale);

VoxelRenderer::VoxelRenderer(int width, int height, int depth, float size)
    : gridWidth(width), gridHeight(height), gridDepth(depth), voxelSize(size),
      displayListsGenerated(false), cameraX(0.0f), cameraY(0.0f), cameraZ(-5.0f),
      rotX(0.0f), rotY(0.0f), densityThreshold(0.5f) {
    
    // Initialize voxel data
    voxelData.resize(gridWidth * gridHeight * gridDepth, 0.0f);
    
    // Generate terrain with a new random seed
    generateTerrain();
}

VoxelRenderer::~VoxelRenderer() {
    // Clean up display lists if needed
    if (displayListsGenerated) {
        deleteDisplayLists();
    }
}

void VoxelRenderer::generateTerrain(uint64_t seed) {
    // If seed is 0, get a random seed in range up to 10 billion
    if (seed == 0) {
        seed = getRandomSeed();
    }
    
    // Store the seed
    currentSeed = seed;
    
    // Reset the noise generator with the new seed
    noiseGenerator.reseed(seed);
    
    // Generate the terrain using fractal noise
    generateFractalTerrain(voxelData, gridWidth, gridHeight, gridDepth, noiseGenerator, 0.1f);
    
    // Regenerate display lists if needed
    if (displayListsGenerated) {
        deleteDisplayLists();
    }
    generateDisplayLists();
    
    std::cout << "Generated terrain with seed: " << seed << std::endl;
}

void VoxelRenderer::regenerateTerrain() {
    generateTerrain(currentSeed);
}

uint64_t VoxelRenderer::getSeed() const {
    return currentSeed;
}

void VoxelRenderer::setDensityThreshold(float threshold) {
    if (threshold != densityThreshold) {
        densityThreshold = threshold;
        
        // Regenerate display lists with new threshold
        if (displayListsGenerated) {
            deleteDisplayLists();
            generateDisplayLists();
        }
    }
}

void VoxelRenderer::moveCamera(float dx, float dy, float dz) {
    cameraX += dx;
    cameraY += dy;
    cameraZ += dz;
}

void VoxelRenderer::rotateCamera(float dx, float dy) {
    rotX += dx;
    rotY += dy;
    
    // Keep rotation in reasonable range
    if (rotX > 360.0f) rotX -= 360.0f;
    if (rotX < 0.0f) rotX += 360.0f;
    
    if (rotY > 89.0f) rotY = 89.0f;
    if (rotY < -89.0f) rotY = -89.0f;
}

void VoxelRenderer::render() {
    // Set up camera
    glLoadIdentity();
    glTranslatef(cameraX, cameraY, cameraZ);
    glRotatef(rotY, 1.0f, 0.0f, 0.0f);
    glRotatef(rotX, 0.0f, 1.0f, 0.0f);
    
    // Center the grid
    float offsetX = -gridWidth * voxelSize / 2.0f;
    float offsetY = -gridHeight * voxelSize / 2.0f;
    float offsetZ = -gridDepth * voxelSize / 2.0f;
    
    glTranslatef(offsetX, offsetY, offsetZ);
    
    if (displayListsGenerated) {
        // Call the display list
        glCallList(displayListBase);
    } else {
        // Generate display lists if not already done
        generateDisplayLists();
    }
}

void VoxelRenderer::generateDisplayLists() {
    // Delete old display lists if they exist
    if (displayListsGenerated) {
        deleteDisplayLists();
    }
    
    // Create new display list
    displayListBase = glGenLists(1);
    displayListsGenerated = true;
    
    glNewList(displayListBase, GL_COMPILE);
    
    // Render all voxels
    for (int z = 0; z < gridDepth; z++) {
        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                if (isVoxelVisible(x, y, z)) {
                    int index = x + y * gridWidth + z * gridWidth * gridHeight;
                    drawVoxel(x * voxelSize, y * voxelSize, z * voxelSize, 
                             voxelSize * 0.95f, voxelData[index]);
                }
            }
        }
    }
    
    glEndList();
}

void VoxelRenderer::deleteDisplayLists() {
    if (displayListsGenerated) {
        glDeleteLists(displayListBase, 1);
        displayListsGenerated = false;
    }
}

void VoxelRenderer::drawVoxel(float x, float y, float z, float size, float density) {
    // Color based on density
    float r = 0.2f + 0.8f * density;
    float g = 0.3f + 0.5f * density;
    float b = 0.8f - 0.6f * density;
    
    glColor3f(r, g, b);
    
    // Draw a cube
    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(x, y, z + size);
    glVertex3f(x + size, y, z + size);
    glVertex3f(x + size, y + size, z + size);
    glVertex3f(x, y + size, z + size);
    
    // Back face
    glVertex3f(x, y, z);
    glVertex3f(x, y + size, z);
    glVertex3f(x + size, y + size, z);
    glVertex3f(x + size, y, z);
    
    // Top face
    glVertex3f(x, y + size, z);
    glVertex3f(x, y + size, z + size);
    glVertex3f(x + size, y + size, z + size);
    glVertex3f(x + size, y + size, z);
    
    // Bottom face
    glVertex3f(x, y, z);
    glVertex3f(x + size, y, z);
    glVertex3f(x + size, y, z + size);
    glVertex3f(x, y, z + size);
    
    // Right face
    glVertex3f(x + size, y, z);
    glVertex3f(x + size, y + size, z);
    glVertex3f(x + size, y + size, z + size);
    glVertex3f(x + size, y, z + size);
    
    // Left face
    glVertex3f(x, y, z);
    glVertex3f(x, y, z + size);
    glVertex3f(x, y + size, z + size);
    glVertex3f(x, y + size, z);
    
    glEnd();
}

bool VoxelRenderer::isVoxelVisible(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0 || 
        x >= gridWidth || y >= gridHeight || z >= gridDepth) {
        return false;
    }
    
    int index = x + y * gridWidth + z * gridWidth * gridHeight;
    return voxelData[index] > densityThreshold;
} 