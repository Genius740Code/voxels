# Fractal Voxel Generator

This application demonstrates 3D voxel rendering using OpenGL and SFML, with procedural terrain generation using fractal Perlin noise.

## Features

- 3D voxel terrain visualization
- Fractal Perlin noise generation with seeds up to 10 billion (10,000,000,000)
- Real-time terrain generation
- Camera controls for exploring the generated terrain
- Adjustable density threshold for terrain visualization

## Controls

- **WASD**: Move camera forward/backward/left/right
- **Q/E**: Move camera up/down
- **Right Mouse Button + Drag**: Rotate camera
- **Space**: Generate new terrain with a random seed
- **+/-**: Adjust density threshold for terrain visualization
- **ESC**: Exit application

## Technical Details

The application uses:
- C++17 with SFML for window management and OpenGL for 3D rendering
- Custom Perlin noise implementation with expanded seed range (up to 10 billion)
- Fractal Brownian Motion (fBm) for multi-octave noise generation
- Display lists for efficient rendering of voxel terrain

## Build and Run

To compile and run the application:

1. Ensure you have the following prerequisites:
   - GCC with C++17 support
   - SFML libraries
   - OpenGL libraries

2. Run the `compile_and_run.bat` script:
   ```
   compile_and_run.bat
   ```

The script will:
- Create the necessary directories
- Copy required assets (fonts)
- Compile the source code
- Copy required DLLs to the bin directory
- Run the application

## Implementation Notes

The Perlin noise implementation features:
- 64-bit seed support for large seed ranges
- Gradient noise with smooth interpolation
- Multiple octaves of noise with configurable persistence and lacunarity
- Efficient permutation table for consistent noise patterns
