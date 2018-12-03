#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>

#include "GraphicsEngine.h"
#include "UI.h"

/**
\mainpage Flight Simulator 2018

\tableofcontents

\section intro Introduction

This program simulates a plane flying through a city. The simulation will start with the
user in the hanger sitting still. When the user is ready they can fly out of the hanger
and control the plane or camera through a city scene. The city will be random each time the
program is run. This program uses the bullet physics engine to create the objects in the scene.
The camera is a Yaw-Pitch&Roll camera with a set velocity to give the user the feel of flying.
The whole world is wrapped in a cub map of a sky.

\subsection options User Options

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- R: Resets the scene.
- C: Resets the camera.
- F1 or Q: Adds in a Cube.
- F2 or W: Adds in a Sphere.
- F10: Saves a screen shot of the graphics window to a png file.

- A: Makes the camera Yaw to the left.
- D: Makes the camera Yaw to the right.
- Up-Arrow: Makes the camera Pitch up.
- Down-Arrow: Makes the camera Pitch down.
- Left-Arrow: Makes the camera Roll left.
- Right-Arrow: Makes the camera Roll right.
- Alt: Holding Alt stops the camera.

\note Note that the shader programs "VertexShaderLighting.glsl" and "PhongMultipleLights.glsl"
are expected to be in the same folder as the executable.  Your graphics card must also be
able to support OpenGL version 3.3 to run this program.


---

\subsection copyright Copyright

\author    Don Spickler
\version   1.2
\date      2/10/2018
\copyright 2018


---

\subsection license License

GNU Public License

This software is provided as-is, without warranty of ANY KIND, either expressed or implied,
including but not limited to the implied warranties of merchant ability and/or fitness for a
particular purpose. The authors shall NOT be held liable for ANY damage to you, your computer,
or to anyone or anything else, that may result from its use, or misuse.
All trademarks and other registered names contained in this package are the property
of their respective owners.  USE OF THIS SOFTWARE INDICATES THAT YOU AGREE TO THE ABOVE CONDITIONS.

*/

/**
\file main.cpp
\brief Main driver for the program.

This is the main program driver that sets up the graphics engine and links it to
the user interface processor.

\author    Don Spickler(Edited by Keith Livingston)
\version   1.2
\date      12/22/2017

*/

/**
\brief The Main function, program entry point.

\return Standard EXIT_SUCCESS return on successful run.

The main function, responsible for initializing OpenGL and setting up
the SFML interface for OpenGL.

*/

int main()
{
    //  Program setup variables.
    std::string programTitle = "Flight Simulator 2018";
    GLint MinMajor = 3;
    GLint MinMinor = 3;
    GLint WindowWidth = 1200;
    GLint WindowHeight = 800;
    bool DisplayInfo = true;

    //  Other variables
    GLint major;
    GLint minor;
    sf::Clock clock;
    sf::Time time = clock.restart();
    long framecount = 0;

    // Try core context of 10.10 (too advanced) and see what card will resort to.
    // For setting up OpenGL, GLEW, and check versions.
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "OpenGL Setup", sf::Style::Default,
                            sf::ContextSettings(24, 8, 4, 10, 10, sf::ContextSettings::Core));

    window.setVisible(false);

#ifndef __APPLE__
    // Turn on GLEW for Windows and Linux.
    glewExperimental = true;
    if (glewInit())
    {
        std::cerr << "\nUnable to initialize GLEW ... exiting. \n";
        exit(EXIT_FAILURE);
    }
#endif // __APPLE__

    //  Get major and minor OpenGL version from graphics card.
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    //  Check card version to minimum required version.
    bool versionOK = true;
    if (major < MinMajor)
        versionOK = false;
    else if (major == MinMajor && minor < MinMinor)
        versionOK = false;

    if (!versionOK)
    {
        std::cerr << "\nGraphics card OpenGL version is " << major << "." << minor << std::endl;
        std::cerr << "Program required OpenGL version is " << MinMajor << "." << MinMinor << std::endl;
        std::cerr << "Exiting\n";
        exit(EXIT_FAILURE);
    }

    //  Output OpenGL version to the console window.
    if (DisplayInfo)
    {
        std::cout << "\n";
        std::cout << "Version  = " << glGetString(GL_VERSION) << "\n";
        std::cout << "Major    = " << major << "\n";
        std::cout << "Minor    = " << minor << "\n";
        std::cout << "Vendor   = " << glGetString(GL_VENDOR) << "\n";
        std::cout << "Renderer = " << glGetString(GL_RENDERER) << "\n";
        std::cout << "\n";
    }
    //  Close setup window and context.
    window.close();

    //  Create graphics engine.
    GraphicsEngine ge(programTitle, major, minor, WindowWidth, WindowHeight);
    UI ui(&ge);

    // Start the Game/GUI loop
    while (ge.isOpen())
    {
        // Call the display function to do the OpenGL rendering.
        ge.display();

        // Process any events.
        ui.processEvents();

        int numOBJ = ge.getNumObjects();
        //  Increment frame counts
        framecount++;

        //  Get Elapsed Time
        float timesec = clock.getElapsedTime().asSeconds();
        char titlebar[1000];

        //  If another second has elapsed, display the FPS and total FPS.
        if (timesec > 1.0)
        {
            float fps = framecount / timesec;
            sprintf(titlebar, "%s     FPS: %.2f   Objects: %d       Use the arrow keys to steer the plane. Use the 'A' Key to roll left, and the 'D' Key to roll right.           Press R to reset the simulation.", programTitle.c_str(), fps, numOBJ);
            ge.setTitle(titlebar);
            time = clock.restart();
            framecount = 0;
        }
    }

    return EXIT_SUCCESS;
}
