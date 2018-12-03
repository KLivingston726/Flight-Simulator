#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Don Spickler(Edited By Keith Livingston)
\version   1.2
\date      2/10/2018

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, loads the shaders, and sets some initial data settings.

*/

GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{
    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");
    useTextureLoc = glGetUniformLocation(program, "useTexture");

    if (SetVS)
    {
        setVerticalSyncEnabled(true);
        setFramerateLimit(60);
    }
    else
    {
        setVerticalSyncEnabled(false);
        setFramerateLimit(0);
    }

    //This is the text. It does not load in properly and hurts the framerate for some reason?

    ttftr1.loadFont("fonts/times.ttf");
    ttftr1.setFontSize(26);
    ttftr1.setColor(0, 0, 0, 1);
    ttftr1.setScreenSize(getSize().x, getSize().y);

    ttftr2.loadFont("fonts/arial.ttf");
    ttftr2.setFontSize(24);
    ttftr2.setColor(0, 0, 0, 1);
    ttftr2.setScreenSize(getSize().x, getSize().y);

    glClearColor(0, 0, 0, 1);

    restitution = 0.5;  // Bounce factor on all objects
    InitPhysics();

    // Load the shaders
    //program = LoadShadersFromFile("VertexShaderLighting.glsl", "PhongMultipleLights.glsl");
    program = LoadShadersFromFile("VertexShaderLightingTexture.glsl", "PhongMultipleLightsAndTexture.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

        CMprogram = LoadShadersFromFile("VertexShaderCubeMap.glsl", "FragmentCubeMap.glsl");

    if (!CMprogram)
    {
        std::cerr << "Could not load Cube Map Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    sf::Image texture;
    bool texloaded = texture.loadFromFile("Textures/stonewall1.jpg");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }



    CMSphere.createSphereOBJ(1000, 20, 20);
    CMSphere.load(0, 1, 2, 3);


    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");
    ModelLoc = glGetUniformLocation(program, "Model");
    NormalLoc = glGetUniformLocation(program, "NormalMatrix");
    texTransLoc = glGetUniformLocation(program, "textrans");

    // Initialize some data.
    mode = GL_FILL;
    sscount = 1;
    glEnable(GL_DEPTH_TEST);

    mat = Materials::redPlastic;

    for (int i = 0; i < 3; i++)
        lt[i].setLight(true,
                       30.0, 30.0, 30.0, 1.0,
                       -1.0, -1.0, -1.0,
                       0.0, 0.0, 0.0, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       0.70, 0.70, 0.70, 1.0,
                       180.0, 0.0,
                       1.0, 0.0, 0.0
                      );

    LoadLights(lt, "Lt", 3);
    loadMaterial(mat);
    glUniform1i(glGetUniformLocation(program, "numLights"), 3);

    glm::vec4 GlobalAmbient(0.2, 0.2, 0.2, 1);
    glUniform4fv(glGetUniformLocation(program, "GlobalAmbient"), 1, glm::value_ptr(GlobalAmbient));

    //  Create a single sphere.
    sphere.createSphereOBJ(1, 10, 10);
    sphere.LoadDataToGraphicsCard(0, 1, 2, 3);

    turnLightsOff("Lt", 10);

    LtPos[0].setTheta(45);
    LtPos[0].setPsi(45);
    LtPos[0].setR(1500);

    LtPos[1].setTheta(100);
    LtPos[1].setPsi(-45);
    LtPos[1].setR(2000);

    LtPos[2].setTheta(-100);
    LtPos[2].setPsi(60);
    LtPos[2].setR(2000);

    for (int i = 0; i < 3; i++)
    {
        lt[i].setPosition(glm::vec4(LtPos[i].getPosition(), 1.0));
        lt[i].setSpotDirection(-LtPos[i].getPosition());
    }

    LoadLights(lt, "Lt", 3);
    turnLightsOn("Lt", 3);

    std::string filename;
    glGenTextures(10, texID);

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
            filename = "Textures/grass4.bmp";
        else if (i == 1)
            filename = "Textures/rock1.bmp";
        else if (i == 2)
            filename = "Textures/stonewall1.jpg";
        else if (i == 3)
            filename = "Textures/wood14.jpg";
        else if (i == 4)
            filename = "Textures/metal024.bmp";

        if (!texture.loadFromFile(filename))
            std::cerr << "Could not load texture: " << filename << std::endl;

        char arrayname[10];
        sprintf(arrayname, "tex[%d]", i);

        //  Link the texture to the shader.
        GLuint tex1_uniform_loc = glGetUniformLocation(program, arrayname);
        glUniform1i(tex1_uniform_loc, i);

        //  Load the texture into texture memory.
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texID[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glUseProgram(CMprogram);

    //  Load in Cube Map
    glActiveTexture(GL_TEXTURE0+10);
    glUniform1i(glGetUniformLocation(CMprogram, "cmtex"), 10);

    // Generate a new cube map texture and bind to it
    glGenTextures (1, &CubeMapTexId);
    glBindTexture (GL_TEXTURE_CUBE_MAP, CubeMapTexId);

    // Setup some parameters for texture filters and mipmapping
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (!texture.loadFromFile("Textures/skybox_right.jpg"))
        std::cerr << "Could not load texture: skybox_right.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    if (!texture.loadFromFile("Textures/skybox_left.jpg"))
        std::cerr << "Could not load texture: skybox_left.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    if (!texture.loadFromFile("Textures/skybox_bottom.jpg"))
        std::cerr << "Could not load texture: skybox_bottom.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    if (!texture.loadFromFile("Textures/skybox_top.jpg"))
        std::cerr << "Could not load texture: skybox_top.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    if (!texture.loadFromFile("Textures/skybox_front.jpg"))
        std::cerr << "Could not load texture: skybox_front.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    if (!texture.loadFromFile("Textures/skybox_back.jpg"))
        std::cerr << "Could not load texture: skybox_back.jpg" << std::endl;
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    resize();
}

/**
\brief Destructor

Call to ExitPhysics to clean up the object allocation.

*/

GraphicsEngine::~GraphicsEngine()
{
    ExitPhysics();
}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen, runs through the set of all Collision Objects,
extracts their transformation, applies the transformation, calculates and applies
the normal transformations, and calls the draw functions of the box and sphere. Also
this function creates the city scene and loads in the rigidBodies to be used in
the program. This function loads in the different materials and text used on the screen
shapes.

*/

void GraphicsEngine::display()
{
    if (!dynamicsWorld) return;

    UpdateScene();
    getCollisions();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Prints the text to the screen
        ttftr2.setFontSize(24);
        ttftr2.setColor(.75, .75, .75, 1);

        int x = (getSize().x - ttftr2.textWidth("Press the up arrow to take off"))/2;

        if (x < 1)
            x = 1;

        ttftr2.draw("Press the up arrow to take off", 400, 10);

        char timetext[100];
        sprintf(timetext, "Time: %.2f", GameClock.getElapsedTime().asSeconds());

        ttftr1.setColor(1, 1, 1, 1);
        ttftr1.setFontSize(18);

        int y = getSize().y - 26;

        if (y < 1)
            y = 1;

        ttftr1.draw(timetext, 20, 770);


    glm::mat4 view = yprcamera->lookAt();

    glUseProgram(CMprogram);
    glUniformMatrix4fv(glGetUniformLocation(CMprogram, "PVM"),
                       1, GL_FALSE, glm::value_ptr(projection*view));

    //Drawing the cube map
    CMSphere.draw();

    glUseProgram(program);

    // Set view matrix via current camera.
    glm::vec3 eye = yprcamera->getPosition();
    glUniform3fv(glGetUniformLocation(program, "eye"), 1, glm::value_ptr(eye));

    // Calculate the PV matrix, eases the calculation per object.
    glm::mat4 PV = projection*view;
    btScalar m[16];
    btTransform trans;

    for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        // Get the object, its shape, and upcast it to a btRigidBody.
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btCollisionShape* bodyshape = obj->getCollisionShape();
        btRigidBody* body = btRigidBody::upcast(obj);

        // Do not process the camera object.
        if (yprcamera != body)
            if (body && body->getMotionState())
            {
                // Get current transformation of the object.
                body->getMotionState()->getWorldTransform(trans);
                trans.getOpenGLMatrix(m);
                glm::mat4 objmodel = glm::make_mat4(m);


                // Set the material of the object by its type.
                if (body == groundObj)
                    loadMaterial(Materials::jade);
                else if (obj->isKinematicObject())
                    loadMaterial(Materials::gold);
                else if (obj->isStaticObject())
                    loadMaterial(Materials::chrome);
                else
                    loadMaterial(Materials::gold);

                // If the object is a box, get its shape, apply the transformation and scaling, calculate normal matrix and draw.
                if (bodyshape->getShapeType() == BOX_SHAPE_PROXYTYPE)
                {
                    turnTexturesOff("useTexture", 10);
                    turnTextureOn("useTexture", 3);

                    const btBoxShape* boxShape = static_cast<const btBoxShape*>(bodyshape);
                    btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
                    btScalar x = 2*halfExtent[0];
                    btScalar y = 2*halfExtent[1];
                    btScalar z = 2*halfExtent[2];

                    textrans = glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 1));

                    glm::mat4 scalemodel = glm::scale(objmodel, glm::vec3(x, y, z));
                    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(PV*scalemodel));

                    // Create the Normal Transformation matrix
                    glm::mat3 nM(scalemodel);
                    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
                    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
                    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));

                    cube.draw();
                }

                // If the object is a sphere, get its radius, apply the transformation and scaling, calculate normal matrix and draw.
                if (bodyshape->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
                {
                    const btSphereShape* sphereShape = static_cast<const btSphereShape*>(bodyshape);
                    float radius = sphereShape->getMargin();  //radius doesn't include the margin, so draw with margin

                    glm::mat4 scalemodel = glm::scale(objmodel, glm::vec3(radius, radius, radius));
                    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(PV*scalemodel));

                    // Create the Normal Transformation matrix
                    glm::mat3 nM(scalemodel);
                    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
                    glUniformMatrix3fv(NormalLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

                    sphere.draw();
                }
            }
    }

    //  Let YPR camera track fps.
    yprcamera->trackSpeed();

    sf::RenderWindow::display();
    printOpenGLErrors();
}

/**
\brief Changes the fill and line mode being used.

*/

void GraphicsEngine::changeMode()
{
    if (mode == GL_FILL)
        mode = GL_LINE;
    else
        mode = GL_FILL;

    glPolygonMode(GL_FRONT_AND_BACK, mode);
}


/**
\brief Saves a screenshot of the current display to a file, ScreenShot###.png.

*/

void GraphicsEngine::screenshot()
{
    char ssfilename[100];
    sprintf(ssfilename, "ScreenShot%d.png", sscount);
    sf::Vector2u windowSize = getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(*this);
    sf::Image img = texture.copyToImage();
    img.saveToFile(ssfilename);
    sscount++;
}

/**
\brief Handles the resizing events of the window.

Sets the viewport to the entire screen and recalculates the projection matrix.
*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    projection = glm::perspective(50.0f*degf, (float)getSize().x/getSize().y, 0.01f, 3000.0f);
}

/**
\brief Sets the size of the rendering window.

\param width --- the width in pixels of the new size.

\param height --- the height in pixels of the new size.

*/

void GraphicsEngine::setSize(unsigned int width, unsigned int height)
{
    sf::RenderWindow::setSize(sf::Vector2u(width, height));
    resize();
}

/**
\brief Returns a pointer to the yaw-pitch-roll camera.

*/

btYPRCamera* GraphicsEngine::getYPRCamera()
{
    return yprcamera;
}

/**
\brief Prints all OpenGL errors to stderr.

*/

void GraphicsEngine::printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major. This is for the 3 X 3 matrices.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat4 m)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major. This is for the 3 X 3 matrices.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat3 m)
{
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
\brief Turns the light on

*/

void GraphicsEngine::turnLightOn()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), true);
}

/**
\brief Turns the light off

*/

void GraphicsEngine::turnLightOff()
{
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "Lt.on"), false);
}

/**
\brief Loads the light structure to the shader light structure.

\param Lt --- Light structure to load.

*/

void GraphicsEngine::loadLight(Light Lt)
{
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "Lt.on"), Lt.getOn());
    glUniform4fv(glGetUniformLocation(program, "Lt.position"), 1, glm::value_ptr(Lt.getPosition()));
    glUniform4fv(glGetUniformLocation(program, "Lt.ambient"), 1, glm::value_ptr(Lt.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Lt.diffuse"), 1, glm::value_ptr(Lt.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Lt.specular"), 1, glm::value_ptr(Lt.getSpecular()));
    glUniform3fv(glGetUniformLocation(program, "Lt.spotDirection"), 1, glm::value_ptr(Lt.getSpotDirection()));
    glUniform3fv(glGetUniformLocation(program, "Lt.attenuation"), 1, glm::value_ptr(Lt.getAttenuation()));
    glUniform1f(glGetUniformLocation(program, "Lt.spotCutoff"), Lt.getSpotCutoff());
    glUniform1f(glGetUniformLocation(program, "Lt.spotExponent"), Lt.getSpotExponent());
}

/**
\brief Loads the material structure to the shader material structure.

\param Mat --- Material structure to load.

*/

void GraphicsEngine::loadMaterial(Material Mat)
{
    glUseProgram(program);

    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, glm::value_ptr(Mat.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, glm::value_ptr(Mat.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, glm::value_ptr(Mat.getSpecular()));
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, glm::value_ptr(Mat.getEmission()));
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), Mat.getShininess());
}

/**
\brief Loads a single light into a light array in the shader.

\param Lt --- Light to load.

\param name --- The name of the array in the shader.

\param i --- The index of the light to load.

*/

void GraphicsEngine::LoadLight(Light Lt, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), Lt.getOn());

    sprintf(locID, "%s[%d].%s", arrayname, i, "position");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getPosition()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "ambient");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAmbient()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "diffuse");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getDiffuse()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "specular");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpecular()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotDirection");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpotDirection()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "attenuation");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAttenuation()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotCutoff");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotCutoff());

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotExponent");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotExponent());
}

/**
\brief Loads the first num entries of a light array to the a light array in the shader.

\param Lt --- Light array to load.

\param name --- The name of the array in the shader.

\param i --- The number of lights to load.

*/

void GraphicsEngine::LoadLights(Light Lt[], std::string name, int num)
{
    for (int i = 0; i < num; i++)
        LoadLight(Lt[i], name.c_str(), i);
}

/**
\brief Turns the light at index i on.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn on.

*/

void GraphicsEngine::turnLightOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), true);
}

/**
\brief Turns the light at index i off.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn off.

*/

void GraphicsEngine::turnLightOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Turns the first num lights on.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn on.

*/

void GraphicsEngine::turnLightsOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOn(name.c_str(), i);
}

/**
\brief Turns the first num lights off.

\param name --- The name of the array in the shader.

\param num --- Number of lights to turn on.

*/

void GraphicsEngine::turnLightsOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnLightOff(name.c_str(), i);
}


/**
\brief Initializes the physics engine and loads in the basic objects.
This is where the city and camera objects are loaded in.

*/

void GraphicsEngine::InitPhysics()
{
    // Setup the engine.
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,0,0));
    //dynamicsWorld->setGravity(btVector3(0,-10,0));

    // Create the ground object and load it into the array of shapes.
    btCollisionShape* ground = new btBoxShape(btVector3(btScalar(1000.),btScalar(2.),btScalar(1000.)));
    collisionShapes.push_back(ground);

    // Set ground attributes, mass 0 for no movement.
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 0));
    btScalar mass(0.);
    btVector3 localInertia(0,0,0);  //  Mass zero objects have no local inertia

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, ground, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.5);
    body->setRollingFriction(0.5);

    // Add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    groundObj = body;

    //Prints the gold box's to the screen
    for(int i = 1; i <=10; i++)
    {
        addCube();
    }

    //Prints the gold spheres to the screen
    for (int i = 1; i <=10; i++)
    {
        addSphere();
    }

    //Adds the Plane House
        addPlaneHouse();

    //Adds the Buildings *********************************************************
        for (int i = 0; i <= 15; i++)
            for (int j = 0; j <= 25; j++)
                addBuilding(-150 + i*55*rand()/RAND_MAX, -20, -300 + j*55*rand()/RAND_MAX, 10, 200*rand()/RAND_MAX, 10, 0., 0.7, 0.7, 0.5, "Cube");


/*
    // Spinners
    spinnerShapes.clear();

    for (int i = 0; i < 3; i++)
    {
        // Create rigid boxes with half extents, actual size(60, 4, 4)
        btCollisionShape* colShape = new btBoxShape(btVector3(30, 2, 2));
        collisionShapes.push_back(colShape);

        // Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(0.f);
        btVector3 localInertia(0,0,0);
        startTransform.setOrigin(btVector3(100*i - 100, 5, 0));

        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        // Make KINEMATIC, can be animated but not affected by gravity.
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

        //  Set options
        body->setActivationState(DISABLE_DEACTIVATION);
        //Bounciness
        body->setRestitution(restitution);
        body->setFriction(0.5);
        body->setRollingFriction(0.5);

        //  Add to world
        dynamicsWorld->addRigidBody(body);
        spinnerShapes.push_back(body);
    }

    */
    //***********************************************************************
    //  Add the camera, the camera is a dynamic rigid body, Sets the (radius, Mass).
    yprcamera = new btYPRCamera(4, 0.00001);
    collisionShapes.push_back(yprcamera->getCollisionShape());
    dynamicsWorld->addRigidBody(yprcamera);

    //  Adding to dynamicsWorld seems to reset the gravity of the rigidBody.
    yprcamera->setGravity(btVector3(0, -10, 0));
    yprcamera->setMass(1.0);
    yprcamera->setLinearVelocity(btVector3(0,0,0));
    //yprcamera->setMass(0.000001);
    //yprcamera->setMass(0);  // Be Careful

    //  Reset camera position
    yprcamera->PositionCamera(950,2,0, -950,2,0, 0,1,0);

}


/**
\brief Returns the number of objects in the set of Collision Objects.

*/

int GraphicsEngine::getNumObjects()
{
    return dynamicsWorld->getNumCollisionObjects();
}

/**
\brief Removes all physics objects from memory.

*/

void GraphicsEngine::ExitPhysics()
{
    // Remove the rigid bodies from the dynamics world and delete them
    for (int i = dynamicsWorld->getNumCollisionObjects()-1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // Delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    // Delete dynamics world
    delete dynamicsWorld;

    // Delete solver
    delete solver;

    // Delete broadphase
    delete overlappingPairCache;

    // Delete dispatcher
    delete dispatcher;

    // Delete configuration
    delete collisionConfiguration;
}

/**
\brief Removes all physics objects from memory, then loads in the starting configuration.
Note that the camera position is not altered.

*/

void GraphicsEngine::ResetScene()
{
    btVector3 campos = yprcamera->getbtPosition();
    btVector3 camview = yprcamera->getbtView();
    btVector3 camup = yprcamera->getbtUpVector();
    ExitPhysics();
    InitPhysics();
    yprcamera->setPosition(campos);
    yprcamera->setView(camview);
    yprcamera->setUp(camup);
    ResetCamera();
}


/**
\brief Resets the position of the camera.

*/

void GraphicsEngine::ResetCamera()
{
    yprcamera->PositionCamera(950,2,0, -950,2,0, 0,1,0);
}


/**
\brief Adds in a single cube into the World.

*/

void GraphicsEngine::addCube()
{
    btCollisionShape* colShape = new btBoxShape(btVector3(2,2,2));
    //btCollisionShape* colShape = new btBoxShape(btVector3(2,3,5));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();
    btScalar mass(1.f);
    btVector3 localInertia(0,0,0);
    colShape->calculateLocalInertia(mass, localInertia);

    //Where the box's will spawn ***************************************************
    startTransform.setOrigin(btVector3(600.0*rand()/RAND_MAX-100,15+150.0*rand()/RAND_MAX,600.0*rand()/RAND_MAX-100));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    rbInfo.m_restitution = restitution;
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.7);
    body->setRollingFriction(0.7);

    dynamicsWorld->addRigidBody(body);
}


/**
\brief Adds in a single building into the World with a random hight(with parameters).

*/

void GraphicsEngine::addBuilding(float cx, float cy, float cz, float xhe, float yhe, float zhe,
                             float m, float f, float rf, float res, std::string name)
{
    btCollisionShape* colShape = new btBoxShape(btVector3(xhe, yhe, zhe));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();
    btScalar mass(m);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    btVector3 localInertia(0,0,0);
    if (m > 0)
        colShape->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(btVector3(cx, cy, cz));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = restitution;
    TypedRigidBody* body = new TypedRigidBody(rbInfo, name);

    body->setRestitution(res);
    body->setFriction(f);
    body->setRollingFriction(rf);
    body->setActivationState(DISABLE_DEACTIVATION);

    dynamicsWorld->addRigidBody(body);
}


/**
\brief Adds in the plane house or the hanger into the World where the plane will start.

*/

void GraphicsEngine::addPlaneHouse()
{
    {
    btCollisionShape* planeHouse = new btBoxShape(btVector3(btScalar(100.),btScalar(60.),btScalar(20.)));
    collisionShapes.push_back(planeHouse);

    // Set ground attributes, mass 0 for no movement.
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(875, 2, 40));
    btScalar mass(0.);
    btVector3 localInertia(0,0,0);  //  Mass zero objects have no local inertia

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, planeHouse, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.5);
    body->setRollingFriction(0.5);

    // Add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    //groundObj = body;

}
//Wall 2
{
    btCollisionShape* planeHouse = new btBoxShape(btVector3(btScalar(100.),btScalar(60.),btScalar(20.)));
    collisionShapes.push_back(planeHouse);

    // Set ground attributes, mass 0 for no movement.
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(875, 2, -40));
    btScalar mass(0.);
    btVector3 localInertia(0,0,0);  //  Mass zero objects have no local inertia

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, planeHouse, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.5);
    body->setRollingFriction(0.5);

    // Add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    //groundObj = body;

}
//Roof
{
    btCollisionShape* planeHouse = new btBoxShape(btVector3(btScalar(100.),btScalar(10.),btScalar(60.)));
    collisionShapes.push_back(planeHouse);

    // Set ground attributes, mass 0 for no movement.
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(875, 60, 0));
    btScalar mass(0.);
    btVector3 localInertia(0,0,0);  //  Mass zero objects have no local inertia

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, planeHouse, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.5);
    body->setRollingFriction(0.5);

    // Add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    //groundObj = body;

}
//Stopper
{
    btCollisionShape* planeHouse = new btBoxShape(btVector3(btScalar(1.),btScalar(4.15),btScalar(20.)));
    collisionShapes.push_back(planeHouse);

    // Set ground attributes, mass 0 for no movement.
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(945, 2, 0));
    btScalar mass(0.);
    btVector3 localInertia(0,0,0);  //  Mass zero objects have no local inertia

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, planeHouse, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.5);
    body->setRollingFriction(0.5);

    // Add the body to the dynamics world
    dynamicsWorld->addRigidBody(body);
    //groundObj = body;

}
}

/**
\brief Adds in a single sphere into the World.

*/

void GraphicsEngine::addSphere()
{
    btCollisionShape* colShape = new btSphereShape(btScalar(2.));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();
    btScalar mass(1.f);
    btVector3 localInertia(0,0,0);
    colShape->calculateLocalInertia(mass,localInertia);

    startTransform.setOrigin(btVector3(600.0*rand()/RAND_MAX-100,15+100.0*rand()/RAND_MAX,600.0*rand()/RAND_MAX-100));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setRestitution(restitution);
    body->setFriction(0.7);
    body->setRollingFriction(0.3);

    //Set velocity of an object. ************************************ set to 0 right now (x, y, z)
    body->setLinearVelocity(btVector3(0,0,0));

    dynamicsWorld->addRigidBody(body);
}


/**
\brief Returns the elapsed time of the bt_clock.

*/
//CLOCK ******************************************************************
btScalar GraphicsEngine::getDeltaTimeMicroseconds()
{
    btScalar dt = (btScalar)bt_clock.getTimeMicroseconds();
    bt_clock.reset();
    return dt;
}

/**
\brief Returns the elapsed time of the bt_clocktotal.

*/

btScalar GraphicsEngine::getTimeMicroseconds()
{
    btScalar t = (btScalar)bt_clocktotal.getTimeMicroseconds();
    return t;
}


/**
\brief Updates the physics object configuration.

*/
//Update the scene or world that the objects are in(Called by display)
void GraphicsEngine::UpdateScene()
{
    //  Get the elapsed and total time of the simulation.
    float ms = getDeltaTimeMicroseconds();
    float tms = getTimeMicroseconds();

    // Step the simulation
    if (dynamicsWorld)
    {
        dynamicsWorld->stepSimulation(ms / 1000000.f);

        // Update the position of the spinners.
        for (int i = 0; i < spinnerShapes.size(); i++)
        {
            btCollisionObject* obj = spinnerShapes[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            btMotionState* mst = body->getMotionState();
            btTransform wtrans;
            mst->getWorldTransform(wtrans);
            btQuaternion q;
            float dir = pow(-1, (i % 2));
            q.setRotation(btVector3(0, 1, 0), 2*dir*PI*tms / 1000000.f);
            wtrans.setRotation(q);
            mst->setWorldTransform(wtrans);
        }

        //  Prune the objects that will never return to the scene if they fall too far off, -100 to be exact
        for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);

            if (body && body->getMotionState())
            {
                btVector3 pos = body->getCenterOfMassPosition();
                if (pos[1] < -1000)
                {
                    delete body->getMotionState();
                    dynamicsWorld->removeCollisionObject(obj);
                    delete obj;
                }
            }
        }
    }
}

/**
\brief Gets the collisions on a single btobject in the World.

*/

void GraphicsEngine::getCollisions(btCollisionObject* targetObj)
{
    targetHits.resize(0);

    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        if ((obA == targetObj) || (obB == targetObj))
        {
            int numContacts = contactManifold->getNumContacts();
            for (int j = 0; j < numContacts; j++)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getDistance() < 0.f)
                {
                    if (obA == targetObj)
                        targetHits.push_back(obB);
                    else
                        targetHits.push_back(obA);
                }
            }
        }
    }
}

/**
\brief Finds all of the objects that are in contact with another object.

All of the objects that are in contact with another object are loaded
into the targetHits array.

*/

void GraphicsEngine::getCollisions()
{
    targetHits.resize(0);

    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f)
            {
                targetHits.push_back(obA);
                targetHits.push_back(obB);
            }
        }
    }
}

void GraphicsEngine::turnTexturesOff(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOff(name.c_str(), i);
}

/**
\brief Turns the ith texture off.

\param name --- The name of the boolean texture array in the shader.
\param i --- Number of the texture to turn off.

*/

void GraphicsEngine::turnTextureOff(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Turns the first num textures on.

\param name --- The name of the boolean texture array in the shader.
\param num --- Number of textures to turn on.

*/

void GraphicsEngine::turnTexturesOn(std::string name, int num)
{
    for (int i = 0; i < num; i++)
        turnTextureOn(name.c_str(), i);
}

/**
\brief Turns the ith texture on.

\param name --- The name of the boolean texture array in the shader.
\param i --- Number of the texture to turn on.

*/

void GraphicsEngine::turnTextureOn(std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d]", arrayname, i);
    glUniform1i(glGetUniformLocation(program, locID), true);
}

