#include "btYPRCamera.h"


/**
\file btYPRCamera.cpp

\brief Implementation file for the btYPRCamera class.

\author    Don Spickler
\version   1.2
\date      2/10/2018

*/

/**
\brief Constructor

Position is at the origin, the view vector is pointing down the negative z-axis,
up is the positive y direction.  Also sets attributes to allow the camera to move about and
interact in the physics scene without rolling, gravity, or bouncing.

*/

btYPRCamera::btYPRCamera(btScalar cameraRadius, btScalar mass)
    : btRigidBody(mass, new btDefaultMotionState(), new btSphereShape(cameraRadius))
{
    setPosition(0, 0, 0);
    setView(0, 0, -1);
    setUp(0, 1, 0);

    btTransform startTransform;
    startTransform.setIdentity();

    btCollisionShape *cs = getCollisionShape();
    cs->setMargin(0.0001);
    btVector3 localInertia(0,0,0);
    if (mass > 0)
        cs->calculateLocalInertia(mass, localInertia);
    startTransform.setOrigin(Position);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    setMotionState(myMotionState);

    setActivationState(DISABLE_DEACTIVATION);
    setGravity(btVector3(0, 0, 0));
    setRestitution(0);
    setFriction(0);
    setRollingFriction(0);
    setDamping(0.99, 0);

    clock.reset();
    framecount = 0;
}

/**
\brief Function to allow the YPR camera track the number of frames per second that the
system is running.  This FPS is used by the yaw, pitch, and roll update functions to help
synchronize the angular camera movements with the linear camera movements that are
controlled by bullet.

*/

void btYPRCamera::trackSpeed()
{
    framecount++;
    btScalar time = clock.getTimeSeconds();
    if (time > 0.1)
    {
        fps = framecount/(time);
        clock.reset();
        framecount = 0;
    }
}

/**
\brief Normalizes the view and up vectors.

*/

void btYPRCamera::Normalize()
{
    View.normalize();
    UpVector.normalize();
}


/**
\brief Sets the position, view and up for the camera.

The method will also normalize the view and up vectors.

\param X --- X coordinate to the camera position.
\param Y --- Y coordinate to the camera position.
\param Z --- Z coordinate to the camera position.
\param vX --- X coordinate to the viewing vector.
\param vY --- Y coordinate to the viewing vector.
\param vZ --- Z coordinate to the viewing vector.
\param upX --- X coordinate to the up vector.
\param upY --- Y coordinate to the up vector.
\param upZ --- Z coordinate to the up vector.

*/

void btYPRCamera::PositionCamera(btScalar X, btScalar Y, btScalar Z,
                                 btScalar vX, btScalar vY, btScalar vZ,
                                 btScalar upX, btScalar upY, btScalar upZ)
{
    setPosition(X, Y, Z);
    setView(vX, vY, vZ);
    setUp(upX, upY, upZ);
    Normalize();
}


/**
\brief Sets the position, view and up for the camera.

The method will also normalize the view and up vectors.

\param pos --- Position vector.
\param view --- View vector.
\param up --- Up vector.

*/

void btYPRCamera::PositionCamera(btVector3 pos, btVector3 view, btVector3 up)
{
    setPosition(pos);
    setView(view);
    setUp(up);
    Normalize();
}


/**
\brief Sets the position for the camera.

\param X --- X coordinate to the camera position.
\param Y --- Y coordinate to the camera position.
\param Z --- Z coordinate to the camera position.

*/

void btYPRCamera::setPosition(btScalar X, btScalar Y, btScalar Z)
{
    Position.setX(X);
    Position.setY(Y);
    Position.setZ(Z);
    Position.setW(1);
    updateSpherePosition();
}

/**
\brief Sets the position for the camera.

\param pos --- Position vector.

*/

void btYPRCamera::setPosition(btVector3 pos)
{
    Position = pos;
    updateSpherePosition();
}

/**
\brief Sets the view vector for the camera.

The method will also normalize the view vector.

\param X --- X coordinate to the viewing vector.
\param Y --- Y coordinate to the viewing vector.
\param Z --- Z coordinate to the viewing vector.

*/

void btYPRCamera::setView(btScalar X, btScalar Y, btScalar Z)
{
    View.setX(X);
    View.setY(Y);
    View.setZ(Z);
    View.setW(1);
    Normalize();
}


/**
\brief Sets the view vector for the camera.

The method will also normalize the view vector.

\param view --- View vector.

*/

void btYPRCamera::setView(btVector3 view)
{
    View = view;
    Normalize();
}


/**
\brief Sets the up vector for the camera.

The method will also normalize the up vector.

\param X --- X coordinate to the up vector.
\param Y --- Y coordinate to the up vector.
\param Z --- Z coordinate to the up vector.

*/

void btYPRCamera::setUp(btScalar X, btScalar Y, btScalar Z)
{
    UpVector.setX(X);
    UpVector.setY(Y);
    UpVector.setZ(Z);
    UpVector.setW(1);
    Normalize();
}

/**
\brief Sets the up vector for the camera.

The method will also normalize the up vector.

\param up --- Up vector.

*/

void btYPRCamera::setUp(btVector3 up)
{
    UpVector = up;
    Normalize();
}


/**
\brief Returns the current position of the camera.

\return The current position of the camera.

*/

btVector3 btYPRCamera::getbtPosition()
{
    return Position;
}


/**
\brief Returns the current viewing vector of the camera.

\return The current viewing vector of the camera.

*/

btVector3 btYPRCamera::getbtView()
{
    return View;
}

/**
\brief Returns the current up vector of the camera.

\return The current up vector of the camera.

*/

btVector3 btYPRCamera::getbtUpVector()
{
    return UpVector;
}


/**
\brief Returns the current position of the camera.

\return The current position of the camera.

*/

glm::vec3 btYPRCamera::getPosition()
{
    glm::vec3 p;
    p.x = Position.getX();
    p.y = Position.getY();
    p.z = Position.getZ();

    return p;
}


/**
\brief Returns the current viewing vector of the camera.

\return The current viewing vector of the camera.

*/

glm::vec3 btYPRCamera::getView()
{
    glm::vec3 v;
    v.x = View.getX();
    v.y = View.getY();
    v.z = View.getZ();

    return v;
}


/**
\brief Returns the current up vector of the camera.

\return The current up vector of the camera.

*/

glm::vec3 btYPRCamera::getUpVector()
{
    glm::vec3 u;
    u.x = UpVector.getX();
    u.y = UpVector.getY();
    u.z = UpVector.getZ();

    return u;
}


/**
\brief Adds to the current pitch of the camera.

\param num --- The degree measurement of the amount to add to the pitch of the camera,
in degrees per second.

*/

void btYPRCamera::addPitch(btScalar num)
{
    btVector3 v = CalculateMotionNormal();
    View = View.rotate(v, num*deg/fps);
    UpVector = UpVector.rotate(v, num*deg/fps);
    Normalize();
}


/**
\brief Adds to the current yaw of the camera.

\param num --- The degree measurement of the amount to add to the yaw of the camera,
in degrees per second.

*/

void btYPRCamera::addYaw(btScalar num)
{
    View = View.rotate(UpVector, num*deg/fps);
    Normalize();
}


/**
\brief Adds to the current roll of the camera.

\param num --- The degree measurement of the amount to add to the roll of the camera,
in degrees per second..

*/

void btYPRCamera::addRoll(btScalar num)
{
    UpVector = UpVector.rotate(View, num*deg/fps);
    Normalize();
}

/**
\brief Returns the lookat matrix for the current position, line of sight, and up
vectors.

\return The lookat matrix for the current position, line of sight, and up vectors.

*/

glm::mat4 btYPRCamera::lookAt()
{
    updatePosition();

    glm::vec3 p, v, u;
    p.x = Position.getX();
    p.y = Position.getY();
    p.z = Position.getZ();

    v.x = View.getX();
    v.y = View.getY();
    v.z = View.getZ();

    u.x = UpVector.getX();
    u.y = UpVector.getY();
    u.z = UpVector.getZ();

    return glm::lookAt(p, p+v, u);
}

/**
\brief Returns the binormal vector for the current position, line of sight, and up
vectors.


\return The binormal vector for the current position, line of sight, and up
vectors, useful for lateral movement.

*/

btVector3 btYPRCamera::CalculateMotionNormal()
{
    btVector3 cp = View.cross(UpVector);
    cp.normalize();
    return cp;
}


/**
\brief Moves the camera forward.

\param num --- Amount to move the camera forward.

*/

void btYPRCamera::moveForward(btScalar num)
{
    Normalize();
    setLinearVelocity(num*View);
}


/**
\brief Moves the camera right.

\param num --- Amount to move the camera to the right.

*/

void btYPRCamera::moveRight(btScalar num)
{
    Normalize();
    setLinearVelocity(num*CalculateMotionNormal());
}


/**
\brief Moves the camera up.

\param num --- Amount to move the camera up.

*/

void btYPRCamera::moveUp(btScalar num)
{
    Normalize();
    setLinearVelocity(num*UpVector);
}

//  Move the camera in general, incorporates all three directions.


/**
\brief Moves the camera forward, right and up all at once.

\param f --- Amount to move the camera forward.
\param r --- Amount to move the camera to the right.
\param u --- Amount to move the camera up.

*/

void btYPRCamera::moveCamera(btScalar f, btScalar r, btScalar u)
{
    Normalize();
    setLinearVelocity(f*View + r*CalculateMotionNormal() + u*UpVector);
}

/**
\brief Updates the position data from Bullet.

*/

void btYPRCamera::updatePosition()
{
    setPosition(getWorldTransform().getOrigin());
}

/**
\brief Updates the camera sphere position data from the Bullet position data.

*/

void btYPRCamera::updateSpherePosition()
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(getbtPosition());
    setWorldTransform(trans);
}

/**
\brief Resets the mass of the camera.

\param m --- new mass of camera, must be > 0.

*/

void btYPRCamera::setMass(btScalar m)
{
    btCollisionShape *cs = getCollisionShape();
    btVector3 localInertia(0,0,0);
    if (m > 0)
        cs->calculateLocalInertia(m, localInertia);
    setMassProps(m, localInertia);
}

/**
\brief Stops the camera movement.

*/

void btYPRCamera::stop()
{
    setLinearVelocity(btVector3(0, 0, 0));
}
