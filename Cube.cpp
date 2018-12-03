#include "Cube.h"

/**
\file Cube.cpp

\brief Implementation file for the Cube class.

\author    Don Spickler
\version   1.2
\date      1/18/2018

*/

/**
\brief Constructor

Simply stores the pointer of the GraphicsEngine.

*/

Cube::Cube()
{
    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);

    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Removes allocated data from the graphics card.

*/

Cube::~Cube()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}

/**
\brief Loads the vertex and color data to the graphics card.

*/

void Cube::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;
    GLuint vNormal = 2;

    GLushort indices[] = {0, 1, 2,
                          2, 3, 0,
                          6, 5, 4,
                          4, 7, 6,
                          10, 9, 8,
                          8, 11, 10,
                          12, 13, 14,
                          14, 15, 12,
                          16, 17, 18,
                          18, 19, 16,
                          22, 21, 20,
                          20, 23, 22
                         };

    GLfloat points[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, -0.5, 0.5, 1,
                        0.5, -0.5, 0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, 0.5, -0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,

                        -0.5, 0.5, 0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, -0.5, 0.5, 1,

                        0.5, -0.5, 0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        -0.5, 0.5, 0.5, 1,
                       };

    GLfloat normals[] = {0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,
                         0, 0, 1,

                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,
                         0, 0, -1,

                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,
                         0, 1, 0,

                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,
                         0, -1, 0,

                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,
                         1, 0, 0,

                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                         -1, 0, 0,
                        };

    GLfloat colors[] = {1, 0, 0,
                        1, 0, 0,
                        1, 0, 0,
                        1, 0, 0,
                        0, 1, 0,
                        0, 1, 0,
                        0, 1, 0,
                        0, 1, 0,
                        0, 0, 1,
                        0, 0, 1,
                        0, 0, 1,
                        0, 0, 1,
                        1, 1, 0,
                        1, 1, 0,
                        1, 1, 0,
                        1, 1, 0,
                        0, 1, 1,
                        0, 1, 1,
                        0, 1, 1,
                        0, 1, 1,
                        1, 0, 1,
                        1, 0, 1,
                        1, 0, 1,
                        1, 0, 1,
                       };

    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
}

/**
\brief Draws the box to the screen.

*/

void Cube::draw()
{
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
}
