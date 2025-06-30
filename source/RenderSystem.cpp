#include "RenderSystem.h"
#include "icon.h"
#include <direct.h>
#include <iostream>
#include <fstream>
#include <cstring>

// Scatic var for store window's state
static GLFWwindow* s_Window = nullptr;

bool RenderSystem::Initialize(int width, int height, const char* title) {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Setting GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    s_Window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!s_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    GLFWimage icon = {};
    icon.width = LHEP_WIDTH;
    icon.height = LHEP_HEIGHT;
    icon.pixels = (unsigned char*)lhep;
    glfwMakeContextCurrent(s_Window);
    glfwSetWindowIcon(s_Window, 1, &icon);
    glfwSwapInterval(1); // Vertical sync

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    std::string fontRelPath = "/MUX_GUI/include/fonts/hpsimplified_bold.ttf";

    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    std::string currentPath(buffer);
    std::cout << "Current working directory: " << currentPath << std::endl;

    // Search fot 'MUX_GUI' in current path
    std::string prefix = "MUX_GUI";
    size_t pos = currentPath.find(prefix);

    if (pos != std::string::npos) {
        std::string basePath = currentPath.substr(0, pos);
        std::string fontPath = basePath + fontRelPath.substr(1);

        std::ifstream file(fontPath);
        if (!file) {
            std::cerr << "Font file not found: " << fontPath << std::endl;
        }
        else {
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 22);
        }
    }
    else {
        std::cerr << "'MUX_GUI' not found in current path." << std::endl;
    }

    // ImGui style
    ImGui::StyleColorsClassic();

    // Setting render platform
    if (!ImGui_ImplGlfw_InitForOpenGL(s_Window, true)) {
        std::cerr << "Failed to init ImGui GLFW backend" << std::endl;
        return false;
    }

    const char* glsl_version = "#version 130";
    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        std::cerr << "Failed to init ImGui OpenGL backend" << std::endl;
        return false;
    }

    return true;
}

void RenderSystem::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (s_Window) {
        glfwDestroyWindow(s_Window);
    }
    glfwTerminate();
}

bool RenderSystem::ShouldClose() {
    return glfwWindowShouldClose(s_Window);
}

void RenderSystem::BeginFrame() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RenderSystem::EndFrame() {
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(s_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Refresh additional windows (if turned on)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(s_Window);
}

// Windows getter
GLFWwindow* RenderSystem::GetWindow() {
    return s_Window;
}