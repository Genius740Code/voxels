#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>

void setPerspective(float fovY, float aspect, float zNear, float zFar)
{
    float f = 1.0f / std::tan(fovY * 3.14159f / 360.0f);
    float aspect_inv = 1.0f / aspect;
    
    float matrix[16] = {0};
    matrix[0] = f * aspect_inv;
    matrix[5] = f;
    matrix[10] = (zFar + zNear) / (zNear - zFar);
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * zFar * zNear) / (zNear - zFar);
    
    glMultMatrixf(matrix);
}

void rungui()
{
    // Create a window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 3D Window", sf::Style::Default, settings);
    
    // Set framerate limit, affects speed
    window.setFramerateLimit(60);
    
    // Initialize OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glViewport(0, 0, window.getSize().x, window.getSize().y);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    setPerspective(45.0f, ratio, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Angle for rotation
    float angle = 0.0f;
    
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
                setPerspective(45.0f, ratio, 0.1f, 100.0f);
            }
            
            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }
        
        // Update rotation angle
        angle += 1.0f;
        if (angle > 360.0f)
            angle -= 360.0f;
        
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Reset modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Move the camera back a bit
        glTranslatef(0.0f, 0.0f, -5.0f);
        
        // Rotate the scene
        glRotatef(angle, 1.0f, 0.5f, 0.0f);
        
        // Draw a colored pyramid
        glBegin(GL_TRIANGLES);
            // Front face (red)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);   // Top
            glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom left
            glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom right
            
            // Right face (green)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);    // Top
            glVertex3f(1.0f, -1.0f, 1.0f);   // Bottom left
            glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom right
            
            // Back face (blue)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);     // Top
            glVertex3f(1.0f, -1.0f, -1.0f);   // Bottom left
            glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom right
            
            // Left face (yellow)
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);     // Top
            glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom left
            glVertex3f(-1.0f, -1.0f, 1.0f);   // Bottom right
        glEnd();
        
        // Draw a square at the bottom (base of pyramid)
        glBegin(GL_QUADS);
            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, 1.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
        glEnd();
        
        // Display what was drawn
        window.display();
    }
} 