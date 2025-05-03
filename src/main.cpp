#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "voxel_renderer.hpp"

void displaySeedInfo(sf::RenderWindow& window, const VoxelRenderer& renderer);

// Helper function to replace gluPerspective
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

int main()
{
    // Create a window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Fractal Voxel Generator", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    
    // Initialize OpenGL
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    perspectiveGL(45.0f, ratio, 0.1f, 500.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Create the voxel renderer with larger grid (64x64x64)
    VoxelRenderer voxelRenderer(64, 64, 64, 0.1f);
    
    // Set initial camera position for a good view
    voxelRenderer.moveCamera(0.0f, 0.0f, -10.0f);
    
    // Camera movement speed
    float moveSpeed = 0.2f;
    
    // Text for displaying info
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "Warning: Could not load font" << std::endl;
    }
    
    // Mouse control
    bool mouseControlActive = false;
    sf::Vector2i lastMousePos;
    
    // Main game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
                
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                GLfloat ratio = static_cast<float>(event.size.width) / event.size.height;
                perspectiveGL(45.0f, ratio, 0.1f, 500.0f);
                
                glMatrixMode(GL_MODELVIEW);
            }
            
            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                
                // Regenerate terrain with a new seed
                if (event.key.code == sf::Keyboard::Space)
                    voxelRenderer.generateTerrain();
                
                // Adjust density threshold with + and -
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal)
                    voxelRenderer.setDensityThreshold(0.6f);
                
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash)
                    voxelRenderer.setDensityThreshold(0.4f);
            }
            
            // Mouse button events
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    mouseControlActive = true;
                    lastMousePos = sf::Mouse::getPosition(window);
                }
            }
            
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    mouseControlActive = false;
                }
            }
            
            // Mouse movement for camera control
            if (event.type == sf::Event::MouseMoved && mouseControlActive) {
                sf::Vector2i currentPos = sf::Mouse::getPosition(window);
                sf::Vector2i delta = currentPos - lastMousePos;
                
                voxelRenderer.rotateCamera(delta.x * 0.2f, delta.y * 0.2f);
                lastMousePos = currentPos;
            }
        }
        
        // Keyboard camera movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            voxelRenderer.moveCamera(0.0f, 0.0f, moveSpeed);
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            voxelRenderer.moveCamera(0.0f, 0.0f, -moveSpeed);
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            voxelRenderer.moveCamera(moveSpeed, 0.0f, 0.0f);
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            voxelRenderer.moveCamera(-moveSpeed, 0.0f, 0.0f);
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            voxelRenderer.moveCamera(0.0f, moveSpeed, 0.0f);
            
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            voxelRenderer.moveCamera(0.0f, -moveSpeed, 0.0f);
        
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render the voxel grid
        voxelRenderer.render();
        
        // Switch to SFML's 2D rendering for UI
        window.pushGLStates();
        
        // Display seed info
        displaySeedInfo(window, voxelRenderer);
        
        window.popGLStates();
        
        // Display what was drawn
        window.display();
    }
    
    return 0;
}

void displaySeedInfo(sf::RenderWindow& window, const VoxelRenderer& renderer) {
    // Create text for displaying seed
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        return; // Unable to load font
    }
    
    sf::Text seedText;
    seedText.setFont(font);
    
    // Create formatted string with the seed value
    std::stringstream ss;
    ss << "Seed: " << std::setw(10) << std::setfill('0') << renderer.getSeed();
    ss << "\nControls: WASD/QE to move, Right-click + drag to rotate";
    ss << "\nSpace = New Seed, +/- = Adjust Density";
    
    // Set the text properties
    seedText.setString(ss.str());
    seedText.setCharacterSize(18);
    seedText.setFillColor(sf::Color::White);
    seedText.setPosition(10.f, 10.f);
    
    // Draw the text
    window.draw(seedText);
} 