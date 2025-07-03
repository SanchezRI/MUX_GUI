#include "RenderSystem.h"
#include "GuiWindows.h"
#include "AppState.h"

int main() {
    // Init
    if (!RenderSystem::Initialize(1280, 720, "MUX GUI Simplified")) {
        return -1;
    }

    AppState state;

    // Main cycle
    while (!RenderSystem::ShouldClose()) {
        RenderSystem::BeginFrame();

        // Render UI
        GuiWindows::ShowMainWindow(state);


        RenderSystem::EndFrame();
    }

    // Shut Down
    RenderSystem::Shutdown();
    return 0;
}