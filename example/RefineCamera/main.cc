#include <glad/glad.h>

#include <GLFW/glfw3.h> // must included after glad

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Camera.h>
#include <Cube.h>
#include <Grid.h>
#include <Log.h>
#include <Shader.h>
#include <Util.h>
#include <mathematics.h>

// 基础框架代码
const char *TITLE = "ArtStudio";
const int WIDTH = 256 * 4;
const int HEIGHT = 160 * 4;
GLFWwindow *window;
int framebufferWidth, framebufferHeight;
double fps;
bool isCursorEntered;
double cursorX, cursorY;

enum State { NONE, LEFT_PRESS, RIGHT_PRESS, SCROLL }; // cursor operation state
State state = NONE;
// end

Grid *grid;
Cube *cube;
Camera camera(vec3(0, 5.0f, 10.0f));

#define SHADER_PATH "Shaders/"

void buildShaders() {}

void init() {
    grid = new Grid();
    cube = new Cube();

    buildShaders();

    glEnable(GL_DEPTH_TEST);

    std::cout << camera.getV() << std::endl;
    std::cout << camera.getP() << std::endl;

    {
        auto some_mat4 = glm::mat4(1, 0, 0, 0, //
                                   0, 1, 7, 0, //
                                   0, 8, 1, 0, //
                                   1, 2, 3, 4  //
        );
        debug("some mat4 %s", glm::to_string(some_mat4).c_str());

        auto V = glm::translate(glm::mat4(1.0f), -glm::vec3(0, 20, 10));
        debug("V %s", glm::to_string(V).c_str());

        auto P =
            glm::perspective(glm::radians(60.0f), 8.0f / 5.0f, 1.0f, 100.0f);
        debug("P %s", glm::to_string(P).c_str());
    }
}

void render() {
    mat4 MVP = camera.getP() * camera.getV() * mat4();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid->render(MVP.m);
    cube->render(MVP.m);
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
            camera.reset();
        } else if (key == GLFW_KEY_A) {
            camera.move(vec3(-1, 0, 0));
        } else if (key == GLFW_KEY_D) {
            camera.move(vec3(1, 0, 0));
        } else if (key == GLFW_KEY_W) {
            camera.move(vec3(0, 0, -1));
        } else if (key == GLFW_KEY_S) {
            camera.move(vec3(0, 0, 1));
        } else if (key == GLFW_KEY_Q) {
            camera.move(vec3(0, 1, 0));
        } else if (key == GLFW_KEY_E) {
            camera.move(vec3(0, -1, 0));
        } else if (key == GLFW_KEY_Z) {
            camera.roll(15);
        } else if (key == GLFW_KEY_C) {
            camera.roll(-15);
        } else if (key == GLFW_KEY_UP) {
            camera.pitch(15);
        } else if (key == GLFW_KEY_DOWN) {
            camera.pitch(-15);
        } else if (key == GLFW_KEY_LEFT) {
            camera.yaw(-15);
        } else if (key == GLFW_KEY_RIGHT) {
            camera.yaw(15);
        }
    }
}

void glfwCursorEnterCallback(GLFWwindow *window, int entered) {
    isCursorEntered = entered;
}

void glfwCursorPosCallback(GLFWwindow *window, double x, double y) {
    switch (state) {
    case LEFT_PRESS: // left mouse dragged
        break;
    case RIGHT_PRESS: // right mouse dragged
        break;
    case SCROLL:
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
}

int main(int argc, char **argv) {
    // {
    //     auto q = quaternion::make(vec3(10, 20, 30));
    //     std::cout << q << std::endl;
    // }

    {
        auto q1 = glm::angleAxis(glm::radians(10.0f), glm::vec3(1, 0, 0));
        auto q2 = glm::angleAxis(glm::radians(20.0f), glm::vec3(0, 1, 0));
        auto q3 = glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 0, 1));
        debug("%s", glm::to_string(q3 * q2 * q1).c_str());

        {
            auto q = glm::quat(glm::radians(glm::vec3(10, 20, 30)));
            debug("%s", glm::to_string(q).c_str());
        }
    }

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
