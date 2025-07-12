#pragma once
#include <GLFW/glfw3.h>
#include "icon.h"
#include <direct.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"

class RenderSystem {
public:
    static bool Initialize(int width = 1280, int height = 720, const char* title = "Application");
    static void Shutdown();

    static bool ShouldClose();
    static void BeginFrame();
    static void EndFrame();

    static GLFWwindow* GetWindow();

private:
    RenderSystem() = delete; // Denied access to init objects
};