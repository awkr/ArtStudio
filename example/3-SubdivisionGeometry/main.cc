#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <GLu.h>
#include <Log.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// screen size
const int WIDTH = 1280;
const int HEIGHT = 960;

const int NUM_X = 40; // total quads on X axis
const int NUM_Z = 40;

const float SIZE_X = 4; // size of plane in world space
const float SIZE_Z = 4;
const float HALF_SIZE_X = SIZE_X / 2.0f;
const float HALF_SIZE_Z = SIZE_Z / 2.0f;

// ripple displacement speed
const float SPEED = 2;

// mesh vertices and indices
glm::vec3 vertices[4];
GLushort indices[6];

GLuint vao, vbo, eab;

// projection and modelview matrices
glm::mat4 P = glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 1.f, 1000.f);
glm::mat4 MV = glm::mat4(1);

// camera transformation variables
int state = 0;
double oldX = 0, oldY = 0;
float rX = 25, rY = -40, dist = -10;

int subdivision = 1;

Shader shader;

void onWindowClosing() {
    shader.deleteProgram();

    glDeleteBuffers(1, &eab);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vao);

    debug("shutdown successfull");
}

void glfwWindowCloseCallback(GLFWwindow *window) { onWindowClosing(); }

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {
    if (state == 0)
        dist *= (1 + (y - oldY) / 60.0f);
    else {
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

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_PERIOD:
            ++subdivision;
            break;
        case GLFW_KEY_COMMA:
            --subdivision;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            onWindowClosing();
            break;
        }
    }
}

void init() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    assert(shader.createProgram("SubdivisionGeometry.vert",
                                "SubdivisionGeometry.frag",
                                "SubdivisionGeometry.geom"));

    shader.use();

    glUniform1i(shader("subdivision"),
                subdivision); // set value at initialization

    shader.unuse();

    vertices[0] = glm::vec3(-5, 0, -5);
    vertices[1] = glm::vec3(-5, 0, 5);
    vertices[2] = glm::vec3(5, 0, 5);
    vertices[3] = glm::vec3(5, 0, -5);

    GLushort *id = &indices[0];
    *id++ = 0;
    *id++ = 1;
    *id++ = 2;
    *id++ = 0;
    *id++ = 2;
    *id++ = 3;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &eab);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // pass triangle vertices to buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);
    // enable vertex attribute array for position
    glEnableVertexAttribArray(shader["vertex"]);
    glVertexAttribPointer(shader["vertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);

    // pass indices to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
                 GL_STATIC_DRAW);

    GL_CHECK_ERROR
}

void display(double time) {
    static const float bgColor[] = {0.35, 0.475, 0.25, 1.0};
    glClearBufferfv(GL_COLOR, 0, bgColor);

    // set the camera viewing transformation
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, dist));
    glm::mat4 Rx = glm::rotate(T, rX, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MV = glm::rotate(Rx, rY, glm::vec3(0.0f, 1.0f, 0.0f));
    MV = glm::translate(MV, glm::vec3(-5, 0, -5));

    shader.use();

    glUniform1i(shader("subdivision"), subdivision);

    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));
    // draw the first mesh
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    MV = glm::translate(MV, glm::vec3(10, 0, 0));
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));
    // draw the second mesh
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    MV = glm::translate(MV, glm::vec3(0, 0, 10));
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));
    // draw the third mesh
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    MV = glm::translate(MV, glm::vec3(-10, 0, 0));
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));
    // draw the fourth mesh
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    shader.unuse();
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
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fatal("glad load gl error");
    }

    info("GLFW: %s", glfwGetVersionString());
    info("Vendor: %s", glGetString(GL_VENDOR));
    info("Renderer: %s", glGetString(GL_RENDERER));
    info("Version: %s", glGetString(GL_VERSION));
    info("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    init();

    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        display(time);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
