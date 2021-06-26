#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <Log.h>
#include <Shader.h>
#include <iostream>

void init() {
    GLuint program;
    assert(createProgram("HelloWorld.vert", "HelloWorld.frag", &program));
    glUseProgram(program);
}

void display() {
    static const float bgColor[] = {0.35, 0.475, 0.25, 1.0};
    glClearBufferfv(GL_COLOR, 0, bgColor);
}

int main() {
    if (!glfwInit()) {
        error("glfw init error");
        return EXIT_FAILURE;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(320, 240, "ArtStudio", nullptr, nullptr);
    if (!window) {
        error("glfw create window error");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        error("glad load gl error");
        return EXIT_FAILURE;
    }

    info("OpenGL %s, GLSL %s", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

    init();

    while (!glfwWindowShouldClose(window)) {
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}