#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLu.h>
#include <Log.h>
#include <Shader.h>
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

enum State { NONE, LEFT_PRESS, RIGHT_PRESS, SCROLL }; // cursor operation state
State state = NONE;
// end

// Camera movement
float fovy = 60.0f; // vertical FOV in degree
float nearPlane = 1.0f;
float farPlane = 100.0f;
float cameraAngleX = 0;  // pitch in degree
float cameraAngleY = 45; // yaw in degree
float cameraDistance = -10;

glm::mat4 MV = glm::mat4(1);
glm::mat4 P = glm::perspective(glm::radians(fovy), (GLfloat)WIDTH / HEIGHT,
                               nearPlane, farPlane);

void rotateCamera(const double x, const double y) {
    cameraAngleY += (x - cursorX);
    cameraAngleX += (y - cursorY);
    cursorX = x;
    cursorY = y;
}

void zoomCamera(const double y) {
    cameraDistance -= (y - cursorY) * 0.1f;
    cursorY = y;
}
// end

GLuint fboId[2];
GLuint depthTexId[2];
GLuint colorTexId[2];
GLuint colorBlenderFboId;
GLuint colorBlenderTexId;

void initFrontPeelingRenderTargets() {
    glGenTextures(2, depthTexId);
    glGenTextures(2, colorTexId);
    glGenFramebuffers(2, fboId);

    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_RECTANGLE, depthTexId[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_DEPTH_COMPONENT,
                     framebufferWidth, framebufferHeight, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, nullptr);

        glBindTexture(GL_TEXTURE_RECTANGLE, colorTexId[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER,
                        GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, framebufferWidth,
                     framebufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_RECTANGLE, depthTexId[i], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_RECTANGLE, colorTexId[i], 0);
    }

    glGenTextures(1, &colorBlenderTexId);
    glBindTexture(GL_TEXTURE_RECTANGLE, colorBlenderTexId);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, framebufferWidth,
                 framebufferHeight, 0, GL_RGBA, GL_FLOAT, 0);

    glGenFramebuffers(1, &colorBlenderFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_RECTANGLE, depthTexId[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_RECTANGLE, colorBlenderTexId, 0);

    GL_CHECK_ERROR

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#define SHADER_PATH "shaders/"

Shader shaderCube;
Shader shaderPeel;
Shader shaderBlend;
Shader shaderFinal;

void buildShaders() {
    shaderCube.attachVertexShader(SHADER_PATH "cube.vert");
    shaderCube.attachFragmentShader(SHADER_PATH "cube.frag");
    assert(shaderCube.link());

    shaderPeel.attachVertexShader(SHADER_PATH "peel.vert");
    shaderPeel.attachFragmentShader(SHADER_PATH "peel.frag");
    assert(shaderPeel.link());

    shaderBlend.attachVertexShader(SHADER_PATH "blend.vert");
    shaderBlend.attachFragmentShader(SHADER_PATH "blend.frag");
    assert(shaderBlend.link());

    shaderFinal.attachVertexShader(SHADER_PATH "final.vert");
    shaderFinal.attachFragmentShader(SHADER_PATH "final.frag");
    assert(shaderFinal.link());
}

GLuint cubeVaoId;
GLuint cubeVboId;
GLuint cubeIndicesId;

void initCubes() {
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

GLuint fullScreenQuadVaoId;
GLuint fullScreenQuadVboId;
GLuint fullScreenQuadIndicesId;

void initFullScreenQuad() {
    glm::vec2 vertices[4];
    vertices[0] = glm::vec2(0, 0);
    vertices[1] = glm::vec2(1, 0);
    vertices[2] = glm::vec2(1, 1);
    vertices[3] = glm::vec2(0, 1);

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &fullScreenQuadVaoId);
    glGenBuffers(1, &fullScreenQuadVboId);
    glGenBuffers(1, &fullScreenQuadIndicesId);

    glBindVertexArray(fullScreenQuadVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullScreenQuadIndicesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
                 GL_STATIC_DRAW);

    GL_CHECK_ERROR
}

void init() {
    initFrontPeelingRenderTargets();
    buildShaders();
    initCubes();
    initFullScreenQuad();
}

glm::vec4 cubeColors[3] = {glm::vec4(1, 0, 0, 0.35), glm::vec4(0, 1, 0, 0.35),
                           glm::vec4(0, 0, 1, 0.35)};

// render normally
void drawCubes(const glm::mat4 &MVP, Shader &shader) {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(cubeVaoId);
    shader.use();
    for (int k = -1; k <= 1; ++k) {     // Z-axis
        for (int j = -1; j <= 1; ++j) { // Y-axis
            int index = 0;
            for (int i = -1; i <= 1; ++i) { // X-axis
                glm::mat4 T = glm::translate(glm::mat4(1),
                                             glm::vec3(i * 2, j * 2, k * 2));
                shader.setUniform4fv("color", 1, &(cubeColors[index++].x));
                shader.setUniformMat4fv("MVP", 1, GL_FALSE,
                                        glm::value_ptr(MVP * T));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
            }
        }
    }
    shader.unuse();

    glBindVertexArray(0);
}

void drawFullScreenQuad() {
    glBindVertexArray(fullScreenQuadVaoId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

// 逻辑变量
bool shouldUseDepthPeeling = false;

GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                        GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                        GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
                        GL_COLOR_ATTACHMENT6};

glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 1);

int numPasses = 4;
// end

void renderFrontPeeling(const glm::mat4 &MVP) {
    if (shouldUseDepthPeeling) {
        // 1 init min depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFboId);
        glDrawBuffer(drawBuffers[0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCubes(MVP, shaderCube);

        // 2 depth peeling + blending
        int numLayers = (numPasses - 1) * 2;
        for (int layer = 1; layer < numLayers; ++layer) {
            int currId = layer % 2;
            int prevId = 1 - currId;

            glBindFramebuffer(GL_FRAMEBUFFER, fboId[currId]);
            glDrawBuffer(drawBuffers[0]);

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);

            shaderPeel.use();
            shaderPeel.setTextureRect("depthTex", depthTexId[prevId], 0);
            drawCubes(MVP, shaderPeel);
            shaderPeel.unuse();

            GL_CHECK_ERROR

            glBindFramebuffer(GL_FRAMEBUFFER, colorBlenderFboId);
            glDrawBuffer(drawBuffers[0]);

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO,
                                GL_ONE_MINUS_SRC_ALPHA);

            shaderBlend.use();
            shaderBlend.setTextureRect("tempTex", colorTexId[currId], 0);
            drawFullScreenQuad();
            shaderBlend.unuse();

            glDisable(GL_BLEND);

            GL_CHECK_ERROR
        }

        // 3 final pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK_LEFT);
        glDisable(GL_DEPTH_TEST);

        shaderFinal.use();
        shaderFinal.setUniform4fv("backgroundColor", 1, &backgroundColor.x);
        shaderFinal.setTextureRect("colorTex", colorBlenderTexId, 0);
        drawFullScreenQuad();
        shaderFinal.unuse();

        GL_CHECK_ERROR
    } else {
        drawCubes(MVP, shaderCube);
    }
}

void render() {
    glm::mat4 T =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, cameraDistance));
    glm::mat4 R =
        glm::rotate(T, glm::radians(cameraAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV =
        glm::rotate(R, glm::radians(cameraAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 MVP = P * MV;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderFrontPeeling(MVP);
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
        } else if (key == GLFW_KEY_SPACE) {
            shouldUseDepthPeeling = !shouldUseDepthPeeling;
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
            state = NONE;
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

    init();

    while (!glfwWindowShouldClose(window)) {
        // calculate FPS
        static double lastUpdateTime = glfwGetTime();
        static int frameCount;
        double currentTime = glfwGetTime();
        if (auto elapsed = currentTime - lastUpdateTime; elapsed >= 1) {
            fps = (double)frameCount / elapsed;
            glfwSetWindowTitle(window,
                               as::stringf("%s FPS %.02f", TITLE, fps).c_str());

            lastUpdateTime = currentTime;
            frameCount = 0;
        } else {
            ++frameCount;
        }

        render();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
