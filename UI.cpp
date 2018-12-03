#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program.

\author    Don Spickler(Edited By Keith Livingston)
\version   1.2
\date      2/10/2018

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
    mouseDown = false;
}

/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
\brief The method handles the SFML event processing and calls the keyboard state processor
method.

This method processes all events in the current SFML event queue and calls the
corresponding processing method. At the end it calls the keyboard state processor
method, outside the event loop.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            keyPressed(event.key);

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();

        if (event.type == sf::Event::MouseMoved)
            processMouseMoved(event.mouseMove);

        if (event.type == sf::Event::MouseButtonPressed)
            processMouseButtonPressed(event.mouseButton);

        if (event.type == sf::Event::MouseButtonReleased)
            processMouseButtonReleased(event.mouseButton);

    }

    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();

}

/**

\param mouseMoveEvent --- The SFML mouse move event structure.

*/

void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{
}

/**
\brief On a left mouse click it does nothing currently.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = true;
        LastPosition.x = mouseButtonEvent.x;
        LastPosition.y = mouseButtonEvent.y;
    }
}

/**
\brief If the left mouse button is released this method will track the release and
exit any drag movement.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = false;
    }
}

/**
\brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

- M: Toggles between fill mode and line mode to draw the triangles.
- R: Resets the scene and the camera. Also R creates a new random city.
- F10: Saves a screen shot of the graphics window to a png file.

*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;

    switch (key)
    {

    case sf::Keyboard::F10:
        ge->screenshot();
        break;

    case sf::Keyboard::M:
        ge->changeMode();
        break;

    case sf::Keyboard::R:
        ge->ResetScene();
        break;

    case sf::Keyboard::C:
        ge->ResetCamera();
        break;

    default:
        break;
    }
}

/**
\brief Calls the yaw-pitch-roll key processor.

*/

void UI::keyboardStateProcessing()
{
    keyboardStateProcessingYPRCamera();
}

/**
\brief The method processes the keyboard state if the yaw-pitch-roll camera is the
one currently being used.

\remark

- A: Makes the camera Yaw to the left.
- D: Makes the camera Yaw to the right.
- Up-Arrow: Makes the camera Pitch up.
- Down-Arrow: Makes the camera Pitch down.
- Left-Arrow: Makes the camera Roll left.
- Right-Arrow: Makes the camera Roll right.
- Alt: Holding Alt stops the camera.

*/

void UI::keyboardStateProcessingYPRCamera()
{
    float yprfactor = 25;

    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    bool shiftdown = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
    bool altdown = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);


    if (altdown || sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return;

    if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            ge->getYPRCamera()->addRoll(yprfactor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            ge->getYPRCamera()->addRoll(-yprfactor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->moveForward(50);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->moveForward(-50);
    }
    else
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            ge->getYPRCamera()->addRoll(40);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            ge->getYPRCamera()->addRoll(-40);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getYPRCamera()->addYaw(yprfactor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getYPRCamera()->addYaw(-yprfactor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->addPitch(40);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->addPitch(-40);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getYPRCamera()->moveRight(-50);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getYPRCamera()->moveRight(50);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->moveUp(50);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->moveUp(-50);


    }
        //Sets the movement of the plane.
        ge->getYPRCamera()->moveForward(45);
}
