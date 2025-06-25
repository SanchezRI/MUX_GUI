#include "GuiWindows.h"

// Style editor for application

void GuiWindows::ShowStyleEditorWindow(AppState& state) {
	ImGui::Begin("Dear ImGui Style Editor", &state.show_style_editor);
	ImGui::ShowStyleEditor();
	ImGui::End();
}