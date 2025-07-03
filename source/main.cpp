#include "RenderSystem.h"
#include "GuiWindows.h"
#include "AppState.h"

int main() {
    // Init
    if (!RenderSystem::Initialize(1280, 720, "MUX GUI")) {
        return -1;
    }

    AppState state;

    // Main cycle
    while (!RenderSystem::ShouldClose()) {
        RenderSystem::BeginFrame();

        // Render UI
        GuiWindows::ShowMainWindow(state);
        if (state.show_adc_dac_window) {
            GuiWindows::ShowAdcDacWindow(state);
        }

        if (state.show_commutator_window) {
            GuiWindows::ShowMultiplexerWindow(state);
        }

        if (state.show_style_editor) {
            GuiWindows::ShowStyleEditorWindow(state);
        }

        if (state.show_packet_tracer_window) {
            GuiWindows::ShowPacketTracerWindow(state);
        }

        RenderSystem::EndFrame();
    }

    // Shut Down
    RenderSystem::Shutdown();
    return 0;
}