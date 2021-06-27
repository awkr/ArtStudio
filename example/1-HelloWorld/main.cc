#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <Log.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define GL_CHECK_ERROR assert(glGetError() == GL_NO_ERROR);

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

// vertices and indices
Vertex vertices[3];
GLushort indices[3];

GLuint vao, vbo, eab;

// projection and modelview matrices
glm::mat4 P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);

Shader shader;

void onWindowClosing() {
    shader.deleteProgram();

    glDeleteBuffers(1, &eab);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vao);

    debug("shutdown successfull");
}

void glfwWindowCloseCallback(GLFWwindow *window) { onWindowClosing(); }

void init() {
    assert(shader.createProgram("HelloWorld.vert", "HelloWorld.frag"));
    shader.use();

    vertices[0] = {glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0)};
    vertices[1] = {glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)};
    vertices[2] = {glm::vec3(1, -1, 0), glm::vec3(0, 0, 1)};

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

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
    glVertexAttribPointer(shader["vertex"], 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), 0);
    // enable vertex attribute array for color
    glEnableVertexAttribArray(shader["color"]);
    glVertexAttribPointer(shader["color"], 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const GLvoid *)offsetof(Vertex, color));

    // pass indices to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0],
                 GL_STATIC_DRAW);

    GL_CHECK_ERROR
}

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        onWindowClosing();
    }
}

void display() {
    static const float bgColor[] = {0.35, 0.475, 0.25, 1.0};
    glClearBufferfv(GL_COLOR, 0, bgColor);

    shader.use();
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(P * MV));
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
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

    auto window = glfwCreateWindow(320, 240, "ArtStudio", nullptr, nullptr);
    if (!window) {
        fatal("glfw create window error");
    }

    glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
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
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();

        handleInput(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
