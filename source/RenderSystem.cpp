#include "RenderSystem.h"

static GLFWwindow* s_Window = nullptr;

bool LoadApplicationFont(ImGuiIO& io, float size_pixels = 22.0f) {

    std::vector<std::string> possiblePaths;

    char buffer[FILENAME_MAX];
    if (_getcwd(buffer, FILENAME_MAX)) {
        std::string currentPath(buffer);
        std::cout << "Current working directory: " << currentPath << std::endl;

        possiblePaths.push_back(currentPath + "/MUX_GUI/include/fonts/hpsimplified_bold.ttf");
        possiblePaths.push_back(currentPath + "/include/fonts/hpsimplified_bold.ttf");

        std::string prefix = "MUX_GUI";
        size_t pos = currentPath.find(prefix);
        if (pos != std::string::npos) {
            std::string basePath = currentPath.substr(0, pos);
            possiblePaths.push_back(basePath + "MUX_GUI/include/fonts/hpsimplified_bold.ttf");
            possiblePaths.push_back(basePath + "MUX_GUI_Simplified/include/fonts/hpsimplified_bold.ttf");
        }
    }

    possiblePaths.push_back("/MUX_GUI/include/fonts/hpsimplified_bold.ttf");
    possiblePaths.push_back("C:/Program Files/MUX GUI Simplified/hpsimplified_bold.ttf");
    possiblePaths.push_back("C:/Program Files (x86)/MUX GUI Simplified/hpsimplified_bold.ttf");

    for (const auto& fontPath : possiblePaths) {
        std::ifstream file(fontPath);
        if (file) {
            file.close();
            ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), size_pixels);
            if (font) {
                std::cout << "Successfully loaded font from: " << fontPath << std::endl;
                return true;
            }
        }
        std::cerr << "Font not found at: " << fontPath << std::endl;
    }

    std::cerr << "Failed to load font from all tried paths." << std::endl;
    return false;
}

bool RenderSystem::Initialize(int width, int height, const char* title) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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
    glfwSwapInterval(1);

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    /*std::string fontRelPath = "/MUX_GUI/include/fonts/hpsimplified_bold.ttf";

    char buffer[FILENAME_MAX];
    _getcwd(buffer, FILENAME_MAX);
    std::string currentPath(buffer);
    std::cout << "Current working directory: " << currentPath << std::endl;

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
    }*/

    LoadApplicationFont(io);

    ImGui::StyleColorsClassic();

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

GLFWwindow* RenderSystem::GetWindow() {
    return s_Window;
}