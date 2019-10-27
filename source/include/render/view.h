#pragma once
#include "../forward/view.fwd.h"
//Dependencies
#include "../forward/billboard.fwd.h"
#include "../forward/world.fwd.h"
#include "../forward/sprite.fwd.h"
#include "../forward/shader.fwd.h"
#include "../forward/model.fwd.h"
#include "../forward/population.fwd.h"
#include "../forward/bot.fwd.h"
#include "../forward/player.fwd.h"
#include "../forward/interface.fwd.h"
#include "../forward/item.fwd.h"


class Picker{
public:
  Shader shaderColorPick;
  glm::mat4 model = glm::mat4(1.0f);
  GLuint vbo, vao;
  void setup();
};

void Picker::setup(){
  shaderColorPick.setup("pick/colorPick.vs", "pick/colorPick.fs");
  shaderColorPick.addAttribute(0, "in_Position");

  //Setup the VAO and stuff
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);

  GLfloat cubewire[48] = {   -0.5,  0.5,  0.5,
                             -0.5,  0.5, -0.5,
                              0.5,  0.5, -0.5,
                              0.5,  0.5,  0.5,
                             -0.5,  0.5,  0.5,
                             -0.5, -0.5,  0.5,
                             -0.5, -0.5, -0.5,
                             -0.5,  0.5, -0.5,
                             -0.5, -0.5, -0.5,
                              0.5, -0.5, -0.5,
                              0.5,  0.5, -0.5,
                              0.5, -0.5, -0.5,
                              0.5, -0.5,  0.5,
                              0.5,  0.5,  0.5,
                              0.5, -0.5,  0.5,
                             -0.5, -0.5,  0.5};

  //Based on what is picked, draw an outline.
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 48*sizeof(GLfloat), &cubewire[0], GL_STATIC_DRAW); //3*4
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

class View{
  public:
    //Initialization
    bool Init();
    void cleanup();
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    ImGuiIO io;
    unsigned int SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 800;
    const unsigned int SHADOW_WIDTH = 1000, SHADOW_HEIGHT = 1000;
    bool fullscreen = false;
    bool vsync = false;

    //GUI Handler
    Interface* interface;
    bool showmenu = false;
    Picker picker;
    bool picked = false;
    glm::vec3 select = glm::vec3(0);
    glm::vec3 hover = glm::vec3(0);

    //Viewposition
    glm::vec3 viewPos = glm::vec3(90, 14, 90);
    glm::vec3 renderDistance = glm::vec3(2, 2, 2);

    //LOD Handling
    int LOD = 4;
    float zoom = 0.01;
    float zoomInc = 0.001;
    bool updateLOD = false;
    glm::vec2 focus = glm::vec2(0.1, 0.9);

    //Loaded Chunk Models
    void loadChunkModels(World &world);  //When chunks are displayed
    void updateChunkModels(World &world);  //When chunks are updated
    std::vector<Model> models;

    //Rendering Targets
    Billboard shadow; //Shadowmap to render to
    Billboard image;  //Image as projected normally - no changes!
    Billboard temp1;  //Switch between render passes, 1
    Billboard temp2;  //Switch between render passes, 2

    //Shaders
    void setupShaders();
    Shader cubeShader;
    Shader depthShader;
    Shader spriteShader;
    Shader billboardShader;
    Shader blurShader;
    Shader itemShader;

    //Render the entire drawing pipeline
    void update();
    void render(World &world, Player &player, Population &population);
    void renderGUI(World &world, Player &player, Population &population);

    //Render Parameters
    int blur = 0; //Degree of Blur
    bool fog = true;
    bool grain = false;
    glm::vec3 fogColor = glm::vec3(1.0f);
    glm::vec3 clickColorBlock = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 hoverColorBlock = glm::vec3(1.0f, 0.0, 0.0f);

    //View Projector (bunch of camera settings here tbh)
    glm::mat4 camera = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::ortho(-(float)SCREEN_WIDTH*zoom, (float)SCREEN_WIDTH*zoom, -(float)SCREEN_HEIGHT*zoom, (float)SCREEN_HEIGHT*zoom, -200.0f, 200.0f);
    //Light Variables
    int lookstate = 1;
    glm::vec3 lightPos = glm::vec3(3.0f, 6.0f, 2.0f);
    glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 0.9f);
    glm::vec3 skyCol = glm::vec3(0.6, 0.9f, 0.8f);
    float rotation = 0.0;
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthProjection = glm::ortho<float>(-80,80,-80,80,-30,100);
    glm::mat4 depthCamera = glm::lookAt(lightPos, glm::vec3(0,0,0), glm::vec3(0,1,0));

    glm::mat4 biasMatrix = glm::mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    //Get the Intersection and Stuff
    glm::vec3 intersect(World world, glm::vec2 mouse);

    //FPS Calculator
    void calcFPS();
    int ticks = 0;
    const int plotSize = 100;
    float FPS = 0.0f;
    float arr[100] = {0};
};
