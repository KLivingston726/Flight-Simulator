#include "Shape.h"

/**
\file Shape.cpp
\brief Base class for shape objects.

The Shape class is the base class for the shapes we will be using in the scene.
Shapes that will be used will be derived from this class, making graphing code a little
cleaner.

\author    Don Spickler
\version   1.2
\date      1/4/2018

*/

/**
\brief Constructor

Base constructor, since there are pure virtual methods in this class, Shape objects
cannot be instantiated.  The derived classes must override all pure virtual methods
and object of those types are to be instantiated.

*/

Shape::Shape()
{
}

/**
\brief Destructor

*/

Shape::~Shape()
{
}
