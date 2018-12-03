#ifndef TYPEDRIGIDBODY_H_INCLUDED
#define TYPEDRIGIDBODY_H_INCLUDED

#include <string>
#include <btBulletDynamicsCommon.h>

/**
\file TypedRigidBody.h
\brief Header file for TypedRigidBody.cpp

\author    Don Spickler
\version   1.2
\date      2/27/2018

*/

/**
\class TypedRigidBody

\brief The TypedRigidBody class extends the btRigidBody class of Bullet.

This allows the user to tag the btRigidBody with a name.  This allows the application to
store a string that names the internal type (or meaning) of the btRigidBody.  To use this
class, simply replace the btRigidBody declarations and constructors with TypedRigidBody.
This class uses the btRigidBodyConstructionInfo as the first parameter of the constructor
and an optional second parameter string for the name.  This class is easily extendable by
simply adding data members and accessor methods.

*/

class TypedRigidBody : public btRigidBody
{
private:
    std::string bodytypename;

public:
    TypedRigidBody(const btRigidBodyConstructionInfo& constructionInfo, std::string name = "");
    ~TypedRigidBody();

    void setName(std::string name);
    std::string getName();
};


#endif // TYPEDRIGIDBODY_H_INCLUDED

