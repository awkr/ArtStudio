#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad
#include <Log.h>

void init() {}

void display() {
    static const float black[] = {0, 0, 0, 0};
    glClearBufferfv(GL_COLOR, 0, black);
}

int main() {
    if (!glfwInit()) {
        error("glfw init error");
        return EXIT_FAILURE;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
