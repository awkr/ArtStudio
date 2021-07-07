#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad

#include <glm/gtc/type_ptr.hpp>

#include "ParticleSystem.h"
#include <Camera.h>
#include <Log.h>
#include <Shader.h>
#include <Texture.h>
#include <Util.h>

// 基础框架代码
const char *TITLE = "ArtStudio";
const int WIDTH = 1024;
const int HEIGHT = 768;
GLFWwindow *window;
int framebufferWidth, framebufferHeight;
double fps;
bool isCursorEntered;
double cursorX, cursorY;

enum State {
    STATE_NONE,
    LEFT_PRESS,
    RIGHT_PRESS,
    SCROLL
}; // cursor operation state
State state = STATE_NONE;
// end

// Camera
Camera camera(-2, 60, (float)WIDTH / HEIGHT, 1, 100, 0, 45);
glm::mat4 MV = glm::mat4(1);

void rotateCamera(const double x, const double y) {
    camera.rotate(x - cursorX, y - cursorY);
    cursorX = x;
    cursorY = y;
}

void zoomCamera(const double y) {
    camera.zoom((y - cursorY) * 0.1f);
    cursorY = y;
}
// end

ParticleSystem particleSystem;

GLuint cubeVaoId;
GLuint cubeVboId;
GLuint cubeIndicesId;

void initCube() {
    glGenVertexArrays(1, &cubeVaoId);
    glGenBuffers(1, &cubeVboId);
    glGenBuffers(1, &cubeIndicesId);

    // unit cube vertices
    glm::vec3 vertices[8] = {
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),   glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),    glm::vec3(-0.5f, 0.5f, 0.5f)};

    GLushort indices[36] = {0, 5, 4, 5, 0, 1, 3, 7, 6, 3, 6, 2,
                            7, 4, 6, 6, 4, 5, 2, 1, 3, 3, 1, 0,
                            3, 0, 7, 7, 0, 4, 6, 5, 2, 2, 5, 1};
    glBindVertexArray(cubeVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
                 GL_STATIC_DRAW);

    GL_CHECK_ERROR

    glBindVertexArray(0);
}

#define SHADER_PATH "../../Shaders/"

Shader shaderCube;
Shader shaderQuad;

void buildShaders() {
    shaderCube.init();
    shaderCube.attachVertexShader(SHADER_PATH "Cube.vert");
    shaderCube.attachFragmentShader(SHADER_PATH "Cube.frag");
    assert(shaderCube.link());

    shaderQuad.init();
    shaderQuad.attachVertexShader(SHADER_PATH "Quad.vert");
    shaderQuad.attachFragmentShader(SHADER_PATH "Quad.frag");
    assert(shaderQuad.link());
}

void drawCube(const glm::mat4 &VP) {
    static auto color = glm::vec4(1, 0, 0, 1);

    glBindVertexArray(cubeVaoId);
    shaderCube.use();
    auto M = glm::translate(glm::mat4(1), glm::vec3(-2, 0, 0));
    shaderCube.setUniform4fv("color", &(color.x));
    shaderCube.setUniformMat4fv("MVP", glm::value_ptr(VP * M));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    shaderCube.unuse();
    glBindVertexArray(0);
}

GLuint quadVaoId;
GLuint quadVboId;
GLuint quadIndicesId;

void initQuad() {
    glm::vec2 vertices[4];
    vertices[0] = glm::vec2(0, 0);
    vertices[1] = glm::vec2(1, 0);
    vertices[2] = glm::vec2(1, 1);
    vertices[3] = glm::vec2(0, 1);

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &quadVaoId);
    glGenBuffers(1, &quadVboId);
    glGenBuffers(1, &quadIndicesId);

    glBindVertexArray(quadVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, quadVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
                 GL_STATIC_DRAW);

    GL_CHECK_ERROR
}

Texture *tex;

void drawQuad(const glm::mat4 &VP) {
    glBindVertexArray(quadVaoId);
    shaderQuad.use();
    auto M = glm::mat4(1);
    shaderQuad.setUniformMat4fv("MVP", glm::value_ptr(VP * M));
    shaderQuad.setTexture2D("tex", 0, tex->getId());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    shaderQuad.unuse();
}

void init() {
    tex = new Texture("fireworks_red.jpg");
    initCube();
    initQuad();

    buildShaders();
    particleSystem.init(glm::vec3(0, 0, 0));
}

void render(double deltaTime) {
    glClearColor(0.2, 0.4, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 VP = camera.getP() * camera.getV();

    // drawCube(VP);
    // drawQuad(VP);
    particleSystem.render(deltaTime, VP, camera.getPosition());
}

// GLFW callbacks

void glfwWindowCloseCallback(GLFWwindow *window) {}

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

void glfwCursorEnterCallback(GLFWwindow *window, int entered) {
    isCursorEntered = entered;
}

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {
    switch (state) {
    case LEFT_PRESS: // left mouse dragged
        rotateCamera(x, y);
        break;
    case RIGHT_PRESS: // right mouse dragged
        zoomCamera(y);
        break;
    case SCROLL:
        debug("scroll ???");
        break;
    default:
        break;
    }
}

void glfwMouseButtonCallback(GLFWwindow *window, int button, int action,
                             int mods) {
    if (action == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            state = LEFT_PRESS;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            state = RIGHT_PRESS;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
        case GLFW_MOUSE_BUTTON_RIGHT:
            state = STATE_NONE;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            break;
        }
    }
}

void glfwScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    state = SCROLL;
    // or: zoomCamera(cursorY + yoffset);
    rotateCamera(cursorX + xoffset, cursorY + yoffset);
}

int main(int argc, char **argv) {
    if (!glfwInit()) {
        fatal("glfw init error");
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
        !window) {
        glfwTerminate();
        fatal("glfw create window error");
    }

    glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetCursorEnterCallback(window, glfwCursorEnterCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fatal("glad load gl error");
    }

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    as::logGLInformation();

    init();

    while (!glfwWindowShouldClose(window)) {
        // calculate FPS
        static double lastFPSTime = 0;
        static int frameCount;
        auto currFPSTime = glfwGetTime();
        if (auto elapsed = currFPSTime - lastFPSTime; elapsed >= 1) {
            fps = (double)frameCount / elapsed;
            glfwSetWindowTitle(window,
                               as::stringf("%s FPS %.02f", TITLE, fps).c_str());

            lastFPSTime = currFPSTime;
            frameCount = 0;
        } else {
            ++frameCount;
        }

        static double lastRenderTime = 0;
        auto currRenderTime = glfwGetTime();
        render(currRenderTime - lastRenderTime);
        lastRenderTime = currRenderTime;

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
