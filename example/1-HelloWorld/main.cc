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

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void display() {
    static const float bgColor[] = {0.35, 0.475, 0.25, 1.0};
    glClearBufferfv(GL_COLOR, 0, bgColor);
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
