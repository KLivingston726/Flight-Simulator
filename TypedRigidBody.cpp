#include "TypedRigidBody.h"

/**
\file TypedRigidBody.cpp
\brief Implementation file for the TypedRigidBody class.

\author    Don Spickler
\version   1.1
\date      Written: 4/20/2016  <BR> Revised: 4/20/2016

*/

/**
\brief Constructor

\param constructionInfo --- btRigidBodyConstructionInfo class that is usually loaded in the btRigidBody
constructor.

\param name --- Optional string to be stored.

*/

TypedRigidBody::TypedRigidBody(const btRigidBodyConstructionInfo& constructionInfo, std::string name) :
    btRigidBody(constructionInfo)
{
    bodytypename = name;
}

/**
\brief Destructor

*/

TypedRigidBody::~TypedRigidBody()
{}

/**
\brief Sets the name of the object.

\param name --- String to be stored.

*/

void TypedRigidBody::setName(std::string name)
{
    bodytypename = name;
}

/**
\brief Returns the name of the object.

*/

std::string TypedRigidBody::getName()
{
    return bodytypename;
}

