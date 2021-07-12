#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <Log.h>

const int WIDTH = 640;
const int HEIGHT = 480;
GLFWwindow *window;

bool isCursorEntered;

void init() {}

void render() {}

// GLFW callbacks

void glfwWindowCloseCallback(GLFWwindow *window) {}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action,
                     int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void glfwCursorEnterCallback(GLFWwindow *window, int entered) {
    isCursorEntered = entered;
}

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {}

void glfwMouseButtonCallback(GLFWwindow *window, int button, int action,
                             int mods) {}

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
        render();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return EXIT_SUCCESS;
}
