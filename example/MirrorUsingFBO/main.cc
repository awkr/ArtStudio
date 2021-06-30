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

#define GL_CHECK_ERROR assert(glGetError() == GL_NO_ERROR);

Grid *grid;
UnitCube *cube;
Quad *mirror;

// projection and modelview matrices
glm::mat4 P = glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 1.f, 1000.f);
glm::mat4 MV = glm::mat4(1);

// camera transformation variables
int state = 0;
double oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -10;

// FBO and render buffer object ID
GLuint fbo, rb;

// offscreen render texture
GLuint tex;

// local rotation matrix
glm::mat4 localR = glm::mat4(1);

void onWindowClosing() { debug("shutdown successfull"); }

void glfwWindowCloseCallback(GLFWwindow *window) { onWindowClosing(); }

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {
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

void initFBO() {
    // generate and bind fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    // generate and bind render buffer
    glGenRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);

    // set the render buffer storage
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, WIDTH, HEIGHT);

    // generate the offscreen texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, nullptr);

    // bind the tex as colour attachment of FBO
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, tex, 0);
    // set the render buffer as the depth attachment of FBO
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, rb);

    // check for frame buffer completeness status
    if (auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        status == GL_FRAMEBUFFER_COMPLETE) {
        debug("FBO setup succeeded");
    } else {
        debug("FBO setup failed");
    }

    // unbind the texture and FBO
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void init() {
    glEnable(GL_DEPTH_TEST);

    // create the 20 x 20 grid on XZ plane
    grid = new Grid(20, 20);

    cube = new UnitCube();

    // create a quad as mirror object at Z = -2 position
    mirror = new Quad(-2);

    initFBO();
}

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        onWindowClosing();
    }
}

void display() {
    // set the camera transformation
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
    glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 Ry = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MV = Ry;
    glm::mat4 MVP = P * MV;

    // static const float bgColor[] = {0.35, 0.475, 0.25, 1.0};
    // glClearBufferfv(GL_COLOR, 0, bgColor);

    // glClear(GL_DEPTH_BUFFER_BIT);

    // clear the colour and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render scene normally

    grid->render(glm::value_ptr(MVP));

    // move the unit cube on Y axis to bring it to ground level
    localR[3][1] = 0.5;
    cube->render(glm::value_ptr(P * MV * localR));

    // store the current modelview matrix
    auto oldMV = MV;

    // now change the view matrix to where the mirror is reflect the view vector
    // in the mirror normal direction restore the old modelview matrix
    auto V = glm::vec3(-MV[2][0], -MV[2][1], -MV[2][2]);
    auto R = glm::reflect(V, mirror->getNormal());

    // place the virtual camera at the mirror position
    MV = glm::lookAt(mirror->getPosition(), mirror->getPosition() + R,
                     glm::vec3(0, 1, 0));

    // since mirror image is laterally inverted, we multiply the MV matrix by
    // (-1, 1, 1)
    MV = glm::scale(MV, glm::vec3(-1, 1, 1));

    // enable FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    // render to color attachment 0
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    // clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // show the mirror from the font side only
    if (glm::dot(V, mirror->getNormal()) < 0) {
        grid->render(glm::value_ptr(P * MV));
        cube->render(glm::value_ptr(P * MV * localR));
    }

    // unbind the FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // restore the default back buffer
    glDrawBuffer(GL_BACK_LEFT);

    // restore the old modelview matrix
    MV = oldMV;

    // bind the FBO output at the current texture
    glBindTexture(GL_TEXTURE_2D, tex);

    mirror->render(glm::value_ptr(P * MV)); // render mirror
}

int main() {
    if (!glfwInit()) {
        fatal("glfw init error");
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window =
        glfwCreateWindow(WIDTH, HEIGHT, "ArtStudio", nullptr, nullptr);
    if (!window) {
        fatal("glfw create window error");
    }

    glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
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
