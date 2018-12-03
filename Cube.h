#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ProgramDefines.h"
#include "Shape.h"

/**
\file Cube.h

\brief Header file for Cube.cpp

\author    Don Spickler
\version   1.2
\date      1/18/2018

*/

/**
\class Cube

\brief The cube class draws a cube of side lengths 1 centered at the origin.

*/

class Cube : public Shape
{
private:
    GLuint vboptr;   ///< ID for faces VBO.
    GLuint eboptr;   ///< ID for faces index array.
    GLuint bufptr;   ///< ID for faces array buffer.

    void LoadDataToGraphicsCard();

public:
    Cube();
    ~Cube();

    void draw();
};

#endif // CUBE_H_INCLUDED
