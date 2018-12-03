#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

/**
\file Shape.h

\brief Header file for Shape.cpp

\author    Don Spickler
\version   1.2
\date      1/4/2018

*/

/**
\class Shape

\brief The Shape class is the base class for the shapes we will be using in the scene.
Shapes that will be used will be derived from this class, making graphing code a little
cleaner.

*/

class Shape
{
private:

protected:

public:
    Shape();
    ~Shape();

    virtual void LoadDataToGraphicsCard() = 0;
    virtual void draw() = 0;
};

#endif // SHAPE_H_INCLUDED
