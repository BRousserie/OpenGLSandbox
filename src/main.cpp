#define GLFW_INCLUDE_NONE

#include "shader.h"
#include "drawbuffer.h"
#include "renderengine.h"
#include "camera.h"

#include <GLFW/glfw3.h>
#include <glad.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define SHOW_IMGUI_DEMO_WINDOW() 0

#define COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

namespace {
    bool checkOpenGlError() {
        int error;
        if ((error = glGetError()) != GL_NO_ERROR)
        {
            char const* errorString;
            switch (error)
            {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            default:
                errorString = "UNKNOWN";
                break;
            }
            fprintf(stdout, "OpenGL Error: %s\n", errorString);
            assert(false);
        }
        return error != GL_NO_ERROR;
    }

    struct GUIStates {
        bool panLock;
        bool turnLock;
        bool zoomLock;
        int lockPositionX;
        int lockPositionY;
        int camera;
        double time;
        bool playing;

        static constexpr float GUIStates::MOUSE_PAN_SPEED = 0.001f;
        static constexpr float GUIStates::MOUSE_ZOOM_SPEED = 0.05f;
        static constexpr float GUIStates::MOUSE_TURN_SPEED = 0.005f;
    };

    void init_gui_states(GUIStates& guiStates) {
        guiStates.panLock = false;
        guiStates.turnLock = false;
        guiStates.zoomLock = false;
        guiStates.lockPositionX = 0;
        guiStates.lockPositionY = 0;
        guiStates.camera = 0;
        guiStates.time = 0.0;
        guiStates.playing = false;
    }

    struct Render3DUserData {
        DrawBuffer3D cube;
        glm::vec3 cubePosition;
    };

    void createRender3DUserData(Render3DUserData& userData) {
        glm::vec3 vertices[8] =
        {
            { -1, -1, -1},
            { +1, -1, -1},
            { +1, +1, -1},
            { -1, +1, -1},
            { -1, -1, +1},
            { +1, -1, +1},
            { +1, +1, +1},
            { -1, +1, +1},
        };

        glm::vec3 normals[6] =
        {
            { 0, 0, +1},
            { +1, 0, 0},
            { 0, 0, -1},
            { -1, 0, 0},
            { 0, +1, 0},
            { 0, -1, 0},
        };

        int indices[6 * 6] =
        {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        constexpr size_t vertexCount = 36;
        float vertexBuffer[3 * vertexCount];
        for (int i = 0; i < 36; i++) {
            vertexBuffer[i * 3 + 0] = vertices[indices[i]].x;
            vertexBuffer[i * 3 + 1] = vertices[indices[i]].y;
            vertexBuffer[i * 3 + 2] = vertices[indices[i]].z;
        }


        float normalBuffer[3 * vertexCount];
        for (int i = 0; i < 36; i++) {
            normalBuffer[i * 3 + 0] = normals[indices[i / 6]].x;
            normalBuffer[i * 3 + 1] = normals[indices[i / 6]].y;
            normalBuffer[i * 3 + 2] = normals[indices[i / 6]].z;
        }
        
        glm::vec4 cube_colors[vertexCount];
        for (int iColor = 0; iColor < 6 * 6; ++iColor) {
            cube_colors[iColor].r = (iColor % 3) == 0;
            cube_colors[iColor].g = (iColor % 3) == 1;
            cube_colors[iColor].b = (iColor % 3) == 2;
            cube_colors[iColor].a = 0.5f;
        }

        CreateDrawBuffer3DParams params;
        params.pVertices = reinterpret_cast<glm::vec3*>(vertexBuffer);
        params.pNormals = reinterpret_cast<glm::vec3*>(normalBuffer);
        params.pColors = cube_colors;
        params.vertexCount = vertexCount;
        createDrawBuffer3D(userData.cube, params);

        userData.cubePosition = {};
    }

    void onRender3D(const Api3D& api, const RenderParams& params, void* pUserData) {
        const Render3DUserData& userData = *reinterpret_cast<Render3DUserData const*>(pUserData);
        glm::mat4 cubeModelMatrix = glm::translate(glm::identity<glm::mat4>(), userData.cubePosition);
        drawBuffer3D(eDrawMode::Triangles, api, params, userData.cube, &cubeModelMatrix);
    }

    struct Render2DUserData {
        DrawBuffer2D square;
    };

    void createRender2DUserData(Render2DUserData& userData) {
        const float s = 0.5;
        glm::vec2 square_vertices[] = {
            {-s, -s},
            {+s, -s},
            {+s, -s},
            {+s, +s},
            {+s, +s},
            {-s, +s},
            {-s, +s},
            {-s, -s},
        };
        glm::vec4 square_colors[COUNTOF(square_vertices)];
        for (int iColor = 0; iColor < COUNTOF(square_colors); ++iColor) {
            square_colors[iColor].r = (iColor % 3) == 0;
            square_colors[iColor].g = (iColor % 3) == 1;
            square_colors[iColor].b = (iColor % 3) == 2;
            square_colors[iColor].a = 0.5;
        }

        CreateDrawBuffer2DParams params;
        params.pColors = square_colors;
        params.pVertices = square_vertices;
        params.vertexCount = COUNTOF(square_vertices);
        createDrawBuffer2D(userData.square, params);
    }

    void onRender2D(const Api2D& api, const RenderParams& params, void* pUserData) {
        const Render2DUserData& userData = *reinterpret_cast<Render2DUserData const*>(pUserData);
        drawBuffer2D(eDrawMode::Lines, api, userData.square);
    }
}

// Main

int main(int argc, char** argv) {
    // Initialize glfw library
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init glfw");
        system("pause");
        return -1;
    }

#define ERROR(_ERROR_)\
    glfwTerminate();\
    fprintf(stderr, #_ERROR_);\
    fprintf(stderr, "\n");\
    system("pause");\
    return -1;

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    int const DPI = 2; // For retina screens only
#else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    int const DPI = 1;
# endif

    int width = 1024, height = 768;
    float widthf = (float)width;
    float heightf = (float)height;
    double t;
    double fps = 0.0;

    char const* projectName = argv[0];
    char windowName[512 * 2];
    strcpy(windowName, projectName);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width / DPI, height / DPI, windowName, NULL, NULL);
    if (!window) {
        ERROR("Failed to create window");
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Create OpenGL context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("Failed to initialize OpenGL context.");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    // Init gui structures
    Camera camera = {};
    cameraCreate(camera);

    GUIStates guiStates;
    init_gui_states(guiStates);
    float dummySlider = 0.f;

    RenderEngine renderEngine;
    if (!createRenderEngine(renderEngine)) {
        ERROR("Failed to create render engine");
    }

    Render3DUserData render3DUserData;
    createRender3DUserData(render3DUserData);

    Render2DUserData render2DUserData;
    createRender2DUserData(render2DUserData);

    RenderParams renderParams;
    renderParams.Render3DCallback = onRender3D;
    renderParams.pRender3DCallbackUserData = &render3DUserData;

    renderParams.Render2DCallback = onRender2D;
    renderParams.pRender2DCallbackUserData = &render2DUserData;

    renderParams.pCamera = &camera;

    renderParams.backgroundColor.r = 0.f;
    renderParams.backgroundColor.g = 0.f;
    renderParams.backgroundColor.b = 0.f;
    renderParams.backgroundColor.a = 1.f;

    renderParams.pointSize = 1.f;
    renderParams.lineWidth = 1.f;

    if (checkOpenGlError()) {
        ERROR("OpenGL Error before launching main loop");
    }

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window) && (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)) {
        t = glfwGetTime();

        // Poll for and process events
        glfwPollEvents();

        // Mouse states
        int leftButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        int rightButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        int middleButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

        if (leftButton == GLFW_PRESS) {
            guiStates.turnLock = true;
        } else {
            guiStates.turnLock = false;
        }

        if (rightButton == GLFW_PRESS) {
            guiStates.zoomLock = true;
        } else {
            guiStates.zoomLock = false;
        }

        if (middleButton == GLFW_PRESS) {
            guiStates.panLock = true;
        } else {
            guiStates.panLock = false;
        }

        // Camera movements
        int leftAltPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
        int rightAltPressed = glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
        const bool altPressed = leftAltPressed == GLFW_PRESS || rightAltPressed == GLFW_PRESS;

        double mousex_d, mousey_d;
        glfwGetCursorPos(window, &mousex_d, &mousey_d);
        int mousex = (int)mousex_d;
        int mousey = (int)mousey_d;

        if (!altPressed && (leftButton == GLFW_PRESS || rightButton == GLFW_PRESS || middleButton == GLFW_PRESS)) {
            guiStates.lockPositionX = mousex;
            guiStates.lockPositionY = mousey;
        }
        if (altPressed) {
            int diffLockPositionX = mousex - guiStates.lockPositionX;
            int diffLockPositionY = mousey - guiStates.lockPositionY;
            if (guiStates.zoomLock) {
                float zoomDir = 0.0;
                if (diffLockPositionX > 0) {
                    zoomDir = -1.f;
                }
                else if (diffLockPositionX < 0) {
                    zoomDir = 1.f;
                }
                cameraZoom(camera, zoomDir * GUIStates::MOUSE_ZOOM_SPEED);
            } else if (guiStates.turnLock) {
                cameraTurn(camera, diffLockPositionY * GUIStates::MOUSE_TURN_SPEED,
                    diffLockPositionX * GUIStates::MOUSE_TURN_SPEED);

            } else if (guiStates.panLock) {
                cameraPan(camera, diffLockPositionX * GUIStates::MOUSE_PAN_SPEED,
                    diffLockPositionY * GUIStates::MOUSE_PAN_SPEED);
            }
            guiStates.lockPositionX = mousex;
            guiStates.lockPositionY = mousey;
        }

        glfwGetFramebufferSize(window, &renderParams.viewportWidth, &renderParams.viewportHeight);

        render(renderEngine, renderParams);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
#if SHOW_IMGUI_DEMO_WINDOW()
        ImGui::ShowDemoWindow();
#endif

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("3D Sandbox");

            ImGui::ColorEdit4("Background color", (float*)&renderParams.backgroundColor, ImGuiColorEditFlags_NoInputs);
            ImGui::SliderFloat("Point size", &renderParams.pointSize, 0.1f, 10.f);
            ImGui::SliderFloat("Line Width", &renderParams.lineWidth, 0.1f, 10.f);

            float fovDegrees = glm::degrees(camera.fov);
            if (ImGui::SliderFloat("Camera field of fiew (degrees)", &fovDegrees, 15, 180)) {
                camera.fov = glm::radians(fovDegrees);
            }

            ImGui::SliderFloat3("Cube Position", (float(&)[3])render3DUserData.cubePosition, -10.f, 10.f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, width, height);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        if (checkOpenGlError()) {
            assert(false);
        }

        double newTime = glfwGetTime();
        fps = 1.0 / (newTime - t);

        sprintf(windowName, "%s - %.0f fps", projectName, fps);
        glfwSetWindowTitle(window, windowName);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
