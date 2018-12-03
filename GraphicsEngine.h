#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

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
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>
#include "TypedRigidBody.h"

#include "LoadShaders.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "btYPRCamera.h"
#include "Material.h"
#include "Light.h"
#include "MaterialPresets.h"
#include "Models.h"
#include "Cube.h"
#include "TextRendererTTF.h"

#include "btModels.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Don Spickler(Edited By Keith Livingston)
\version   1.2
\date      2/10/2018

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    GLenum mode;    ///< Mode, either point, line or fill.
    int sscount;    ///< Screenshot count to be appended to the screenshot filename.
    Cube cube;      ///< Cube Object
    Models sphere;  ///< Model Object

    GLuint ModelLoc;   ///< Location ID of the Model matrix in the shader.
    GLuint NormalLoc;  ///< Location ID of the Normal matrix in the shader.
    GLuint PVMLoc;     ///< Location ID of the PVM matrix in the shader.
    GLuint program;    ///< ID of the shader program.
    GLuint CMprogram;  ///< ID of the cube map shader program.

    btModels CMSphere;     ///< Sphere Object for Cube Map.
    GLuint useTextureLoc;  ///< Location ID of the texture use flag in the shader.
    GLuint texTransLoc;  ///< Location ID of the texture transformation matrix in the shader.


    btYPRCamera* yprcamera;      ///< Yaw-Pitch-Roll Camera that uses the bullet interface.
    SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.

    Material mat;   ///< Material of the current object.
    Light lt[10];   ///< Light object.
    GLuint texID[6];      ///< Texture IDs.
    GLuint CubeMapTexId;  ///< Cube Map Texture ID.
    glm::mat4 textrans;   ///< Texture transformation matrix.

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 model;       ///< Model Matrix

    btClock bt_clock, bt_clocktotal;                           ///< Bullet Clocks for step timing.
    btDefaultCollisionConfiguration *collisionConfiguration;   ///< Bullet Collision Configuration
    btCollisionDispatcher *dispatcher;                         ///< Bullet Collision Dispatcher
    btBroadphaseInterface *overlappingPairCache;               ///< Bullet Broadphase Interface
    btSequentialImpulseConstraintSolver *solver;               ///< Bullet Sequential Impulse Constraint Solver
    btDiscreteDynamicsWorld *dynamicsWorld;                    ///< Bullet Dynamics World
    btAlignedObjectArray<btCollisionShape*> collisionShapes;   ///< Bullet Array for the spheres and boxes.
    btAlignedObjectArray<btCollisionObject*> spinnerShapes;    ///< Bullet Array for the spinning sticks.
    btCollisionObject* groundObj;                              ///< The ground object.
    btScalar restitution;                                      ///< The bounce factor on all objects.

    btAlignedObjectArray<const btCollisionObject*> targetHits; ///< Array of colliding objects.

    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);
    void print_GLM_Matrix(glm::mat3 m);

    btScalar getTimeMicroseconds();
    btScalar getDeltaTimeMicroseconds();
    sf::Clock GameClock;   ///< Clock tracking the amount of time the game is in progress.

    void getCollisions(btCollisionObject* targetObj);
    void getCollisions();

    TextRendererTTF ttftr1;   ///< TextRendererTTF object for graphical text display.
    TextRendererTTF ttftr2;  ///< TextRendererTTF object for graphical text display.

    void ExitPhysics();
    void InitPhysics();

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    GLfloat* getScreenBounds();

    void loadLight(Light Lt);
    void loadMaterial(Material Mat);

    void turnLightOn();
    void turnLightOff();
    void turnLightOn(std::string name, int i);
    void turnLightOff(std::string name, int i);
    void turnLightsOn(std::string name, int num);
    void turnLightsOff(std::string name, int num);
    void turnTexturesOff(std::string name, int num);
    void turnTexturesOn(std::string name, int num);
    void turnTextureOff(std::string name, int i);
    void turnTextureOn(std::string name, int i);


    void LoadLights(Light Lt[], std::string name, int num);
    void LoadLight(Light Lt, std::string name, int i);

    SphericalCamera* getSphericalCamera();
    btYPRCamera* getYPRCamera();

    void addSphere();
    void addCube();
    void addPlaneHouse();
    void ResetScene();
    void ResetCamera();

    void addBuilding(float cx = 0, float cy = 0, float cz = 0, float xhe = 1, float yhe = 1, float zhe = 1,
                 float m = 1, float f = 0.5, float rf = 0.5, float res = 0.5, std::string name = "");

    void UpdateScene();
    int getNumObjects();
};

#endif // GRAPHICSENGINE_H_INCLUDED
