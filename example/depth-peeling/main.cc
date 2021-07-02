#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <Grid.h>
#include <Log.h>
#include <Quad.h>
#include <Shader.h>
#include <UnitCube.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;

GLFWwindow *window;

glm::mat4 MV = glm::mat4(1);
glm::mat4 P = glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 1.f, 1000.f);
glm::mat4 R = glm::mat4(1);

glm::vec4 cubeColors[3] = {glm::vec4(1, 0, 0, 0.25), glm::vec4(0, 1, 0, 0.25),
                           glm::vec4(0, 0, 1, 0.25)};

// fullscreen quad vao and vbos
GLuint quadVAO;
GLuint quadVBO;
GLuint quadEAB;

// cube vertex array and vertex buffer object IDs
GLuint cubeVBO;
GLuint cubeVAO;
GLuint cubeEAB;

// shaders for cube, front-to-back peeling, blending and final rendering
Shader cubeShader, frontPeelShader, blendShader, finalShader;

// camera transformation variables
int state = 0;
double oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -10;

//
GLuint fbos[2];
GLuint colorTexs[2];
GLuint depthTexs[2];

GLuint colorBlenderFBO;
GLuint colorBlenderColorTex;

bool shouldUseDepthPeeling = false;

glm::vec4 bg = glm::vec4(0, 0, 0, 0); // background color

//
void onWindowClosing() {}

//
bool isCursorEntered;

// GLFW callbacks

void glfwWindowCloseCallback(GLFWwindow *window) { onWindowClosing(); }

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (shouldUseDepthPeeling = !shouldUseDepthPeeling;
            shouldUseDepthPeeling) {
            glfwSetWindowTitle(window, "ArtStudio [depth peeling: on]");
        } else {
            glfwSetWindowTitle(window, "ArtStudio");
        }
    }
}

void glfwCursorEnterCallback(GLFWwindow *window, int entered) {
    isCursorEntered = entered;
}

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {
    if (!isCursorEntered)
        return;

    if (state == 0) {
        dist *= (1 + (y - oldY) / 60.0f);
    } else {
        rY += (x - oldX) / 5.0f;
        rX += (y - oldY) / 5.0f;
    }
    oldX = x;
    oldY = y;
}

void glfwMouseButtonCallback(GLFWwindow *window, int button, int action,
                             int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (state = 0; action == GLFW_PRESS) { // left mouse button pressed
            state = 1;
        }
    }
}

//
void initFBO() {
    glGenFramebuffers(2, fbos);  // generate 2 FBO
    glGenTextures(2, colorTexs); // the FBO has two color attachments
    glGenTextures(2, depthTexs); // the FBO has two depth attachments

    for (int i = 0; i < 2; ++i) {
        // init color attachments
        glBindTexture(GL_TEXTURE_RECTANGLE, colorTexs[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, WIDTH, HEIGHT, 0,
                     GL_RGBA, GL_FLOAT, nullptr);

        // init depth attachments
        glBindTexture(GL_TEXTURE_RECTANGLE, depthTexs[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT32F, WIDTH,
                     HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        // bind FBO and attach the color & depth attachments
        glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_RECTANGLE, colorTexs[i], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_RECTANGLE, depthTexs[i], 0);
    }

    // setup the color attachment for color blend FBO
    glGenTextures(1, &colorBlenderColorTex);
    glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderColorTex);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_FLOAT, 0);

    glGenFramebuffers(1, &colorBlenderFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_RECTANGLE, colorBlenderColorTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_RECTANGLE, depthTexs[0], 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init() {
    initFBO();

    // quad
    glm::vec2 quadVerts[4] = {glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1),
                              glm::vec2(0, 1)};
    GLushort quadIndices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEAB);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0],
                 GL_STATIC_DRAW);

    // cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEAB);

    glm::vec3 vertices[8] = {
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),   glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),    glm::vec3(-0.5f, 0.5f, 0.5f)};

    GLushort cubeIndices[36] = {0, 5, 4, 5, 0, 1, 3, 7, 6, 3, 6, 2,
                                7, 4, 6, 6, 4, 5, 2, 1, 3, 3, 1, 0,
                                3, 0, 7, 7, 0, 4, 6, 5, 2, 2, 5, 1};
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0],
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    assert(cubeShader.createProgram("cube.vert", "cube.frag"));

    assert(frontPeelShader.createProgram("front_peel.vert", "front_peel.frag"));
    frontPeelShader.use();
    frontPeelShader.setUniform1i("depthTex", 0);
    frontPeelShader.unuse();

    assert(blendShader.createProgram("blend.vert", "blend.frag"));
    blendShader.use();
    blendShader.setUniform1i("tempTexture", 0);
    blendShader.unuse();

    assert(finalShader.createProgram("blend.vert", "final.frag"));
    finalShader.use();
    finalShader.setUniform1i("colorTex", 0);
    finalShader.unuse();
}

void drawScene(const glm::mat4 &MVP, Shader &shader) {
    // enable alpha blending with over compositing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(cubeVAO);
    shader.use();
    // for all cubes
    for (int k = -1; k <= 1; ++k) {
        for (int j = -1; j <= 1; ++j) {
            int index = 0;
            for (int i = -1; i <= 1; ++i) {
                glm::mat4 T = glm::translate(glm::mat4(1),
                                             glm::vec3(i * 2, j * 2, k * 2));
                shader.setUniform4fv("color", 1, &(cubeColors[index++].x));
                shader.setUniformMat4fv("MVP", 1, GL_FALSE,
                                        glm::value_ptr(MVP * R * T));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
            }
        }
    }
    shader.unuse();
    glBindVertexArray(0); // unbind vertex array object
}

void drawFullScreenQuad() {
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        onWindowClosing();
    }
}

void display() {
    // camera transformation
    glm::mat4 Tr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
    glm::mat4 Rx = glm::rotate(Tr, rX, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP = P * MV;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (shouldUseDepthPeeling) {
        glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBO);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1: in the first pass, render normally with depth test enabled to get
        // the nearest surface
        glEnable(GL_DEPTH_TEST);
        drawScene(MVP, cubeShader);

        // 2: depth peeling + blending pass
        int numLayers = (6 - 1) * 2;

        // for each pass
        for (int layer = 1; layer < numLayers; layer++) {
            int currId = layer % 2;
            int prevId = 1 - currId;

            // bind the current FBO
            glBindFramebuffer(GL_FRAMEBUFFER, fbos[currId]);
            // set the first colour attachment as draw buffer
            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            // set clear colour to black
            glClearColor(0, 0, 0, 0);
            // clear the colour and depth buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // disbale blending and depth testing
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);

            // bind the depth texture from the previous step
            glBindTexture(GL_TEXTURE_RECTANGLE, depthTexs[prevId]);

            // render scene with the front to back peeling shader
            drawScene(MVP, frontPeelShader);

            // bind the colour blender FBO
            glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFBO);
            // render to its first colour attachment
            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            // enable blending but disable depth testing
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            // change the blending equation to add
            glBlendEquation(GL_FUNC_ADD);
            // use separate blending function
            glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO,
                                GL_ONE_MINUS_SRC_ALPHA);

            // bind the result from the previous iteration as texture
            glBindTexture(GL_TEXTURE_RECTANGLE, depthTexs[currId]);
            // bind the blend shader and then draw a fullscreen quad
            blendShader.use();
            drawFullScreenQuad();
            blendShader.unuse();

            // disable blending
            glDisable(GL_BLEND);
        }

        // 3: final render pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK_LEFT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderColorTex);
        finalShader.use();
        finalShader.setUniform4fv("backgroundColor", 1, &bg.x);
        drawFullScreenQuad();
        finalShader.unuse();
    } else {
        glEnable(GL_DEPTH_TEST);
        drawScene(MVP, cubeShader);
    }

    // // move the unit cube on Y axis to bring it to ground level
    // localR[3][1] = 0.5;
    // cube->render(glm::value_ptr(P * MV * localR));

    // // store the current modelview matrix
    // auto oldMV = MV;

    // // now change the view matrix to where the mirror is reflect the view
    // vector
    // // in the mirror normal direction restore the old modelview matrix
    // auto V = glm::vec3(-MV[2][0], -MV[2][1], -MV[2][2]);
    // auto R = glm::reflect(V, mirror->getNormal());

    // // place the virtual camera at the mirror position
    // MV = glm::lookAt(mirror->getPosition(), mirror->getPosition() + R,
    //                  glm::vec3(0, 1, 0));

    // // since mirror image is laterally inverted, we multiply the MV matrix by
    // // (-1, 1, 1)
    // MV = glm::scale(MV, glm::vec3(-1, 1, 1));

    // // enable FBO
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    // // render to color attachment 0
    // glDrawBuffer(GL_COLOR_ATTACHMENT0);
    // // clear the color and depth buffer
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // // show the mirror from the font side only
    // if (glm::dot(V, mirror->getNormal()) < 0) {
    //     grid->render(glm::value_ptr(P * MV));
    //     cube->render(glm::value_ptr(P * MV * localR));
    // }

    // // unbind the FBO
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // // restore the default back buffer
    // glDrawBuffer(GL_BACK_LEFT);

    // // restore the old modelview matrix
    // MV = oldMV;

    // // bind the FBO output at the current texture
    // glBindTexture(GL_TEXTURE_2D, tex);

    // mirror->render(glm::value_ptr(P * MV)); // render mirror
}

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        fatal("glfw init error");
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (window = glfwCreateWindow(WIDTH, HEIGHT, "ArtStudio", nullptr, nullptr);
        !window) {
        fatal("glfw create window error");
    }

    glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetCursorEnterCallback(window, glfwCursorEnterCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fatal("glad load gl error");
    }

    init();

    while (!glfwWindowShouldClose(window)) {
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();

        handleInput(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
