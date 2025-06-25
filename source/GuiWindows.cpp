//
// Created by SanchezRIwork a.k.a. campaigner2 on 4/15/25.
//

#include "GuiWindows.h"

///////////////////////////////////////////////// Main window ///////////////////////////////////////////
void GuiWindows::ShowMainWindow(AppState& state) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x * 0.33f, viewport->Size.y));

    ImGui::Begin("Devices main window", nullptr, state.window_flags);
    ImGui::SeparatorText("This is master window. All windows settings near");

    if (ImGui::TreeNode("Window's settings"))
    {
        ImGui::BulletText("Here is settings to all windows of application.\nAll of setting apply only for one session\n and doesn't never save");

        ImGui::Bullet();
        ImGui::Checkbox("No titlebar", &state.no_titlebar);
        if (state.no_titlebar)        state.window_flags |= ImGuiWindowFlags_NoTitleBar;

        ImGui::SameLine();
        ImGui::Checkbox("No scrollbar", &state.no_scrollbar);
        if (state.no_scrollbar)       state.window_flags |= ImGuiWindowFlags_NoScrollbar;

        ImGui::SameLine();
        ImGui::Checkbox("No menu", &state.no_menu);
        if (!state.no_menu)           state.window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::Bullet();
        ImGui::Checkbox("No move", &state.no_move);
        if (state.no_move)            state.window_flags |= ImGuiWindowFlags_NoMove;

        ImGui::SameLine();
        ImGui::Checkbox("No resize", &state.no_resize);
        if (state.no_resize)          state.window_flags |= ImGuiWindowFlags_NoResize;

        ImGui::SameLine();
        ImGui::Checkbox("No collapse", &state.no_collapse);
        if (state.no_collapse)        state.window_flags |= ImGuiWindowFlags_NoCollapse;

        ImGui::Bullet();
        ImGui::Checkbox("No navigation", &state.no_nav);
        if (state.no_nav)             state.window_flags |= ImGuiWindowFlags_NoNav;

        ImGui::SameLine();
        ImGui::Checkbox("No background", &state.no_background);
        if (state.no_background)      state.window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Bullet();
        ImGui::Checkbox("No bring to front", &state.no_bring_to_front);
        if (state.no_bring_to_front)  state.window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::SameLine();
        ImGui::Checkbox("No docking", &state.no_docking);
        if (state.no_docking)         state.window_flags |= ImGuiWindowFlags_NoDocking;

        ImGui::Bullet();
        ImGui::Checkbox("Unsaved document", &state.unsaved_document);
        if (state.unsaved_document)   state.window_flags |= ImGuiWindowFlags_UnsavedDocument;

        ImGui::TreePop();
    }

    ImGui::Checkbox("Open Style Editor window", &state.show_style_editor);

    ImGui::SeparatorText("Devices list");

    ImGui::Checkbox("Open Analog Digital Converter", &state.show_adc_dac_window);
    ImGui::SetItemTooltip("Opens ADC/DAC Module's management window");
    ImGui::SameLine(); ImGui::Button("Show ADC description");
    ImGui::SetItemTooltip("This is Analog digital converter + Digital Analog converter module");

    ImGui::Checkbox("Open Commutator MUX", &state.show_multiplexer_window);
    ImGui::SetItemTooltip("Opens multiplexer's management window");
    ImGui::SameLine(); ImGui::Button("Show MUX description");
    ImGui::SetItemTooltip("This is Multiplexer module");

    ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(0.0f, 0.0f), "Searching..");

    ImGui::End();
}
/////////////////////////////////////////////// END MAIN ////////////////////////////////////////////////