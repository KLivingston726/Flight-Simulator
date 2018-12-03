#ifndef btYPRCAMERA_H
#define btYPRCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "ProgramDefines.h"

/**
\file btYPRCamera.h

\brief Header file for btYPRCamera.cpp

\author    Don Spickler
\version   1.2
\date      2/10/2018

*/

/**
\class btYPRCamera

\brief Creates a Yaw-Pitch-Roll camera that integrates into the Bullet Physics
world.  The camera is an extension of the btRigidBody class.

*/

class btYPRCamera : public btRigidBody
{
private:
    btVector3 Position;   ///< Camera position.
    btVector3 View;       ///< Direction of the line of sight.
    btVector3 UpVector;   ///< Direction of up.
    btClock clock;        ///< Bullet clock to track program FPS.
    long framecount;      ///< Frame counter to track program FPS.
    float fps;            ///< Program FPS.

public:
    btYPRCamera(btScalar cameraRadius = 1, btScalar mass = 0.000001);

    void PositionCamera(btScalar X, btScalar Y, btScalar Z,
                        btScalar vX, btScalar vY, btScalar vZ,
                        btScalar upX, btScalar upY, btScalar upZ);
    void setPosition(btScalar X, btScalar Y, btScalar Z);
    void setView(btScalar X, btScalar Y, btScalar Z);
    void setUp(btScalar X, btScalar Y, btScalar Z);

    void PositionCamera(btVector3 pos, btVector3 view, btVector3 up);
    void setPosition(btVector3 pos);
    void setView(btVector3 view);
    void setUp(btVector3 up);

    void Normalize();

    glm::vec3 getPosition();
    glm::vec3 getView();
    glm::vec3 getUpVector();

    btVector3 getbtPosition();
    btVector3 getbtView();
    btVector3 getbtUpVector();

    glm::mat4 lookAt();
    btVector3 CalculateMotionNormal();

    void addPitch(btScalar num);
    void addYaw(btScalar num);
    void addRoll(btScalar num);

    void moveForward(btScalar num);
    void moveRight(btScalar num);
    void moveUp(btScalar num);
    void moveCamera(btScalar f, btScalar r, btScalar u);
    void stop();

    void updatePosition();
    void updateSpherePosition();

    void setMass(btScalar m);

    void trackSpeed();
};

#endif
