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

	ImGui::Checkbox("Open ADC/DAC module ", &state.show_adc_dac_window);
	ImGui::SetItemTooltip("Opens ADC/DAC Module's management window");
	ImGui::SameLine(); ImGui::Button("Show ADC description");
	ImGui::SetItemTooltip("This is Analog digital converter + Digital Analog converter module");

	ImGui::Checkbox("Open Commutator MUX", &state.show_commutator_window);
	ImGui::SetItemTooltip("Opens multiplexer's management window");
	ImGui::SameLine(); ImGui::Button("Show MUX description");
	ImGui::SetItemTooltip("This is Multiplexer module");

	ImGui::Checkbox("Open Packet Tracer PT ", &state.show_packet_tracer_window);
	ImGui::SetItemTooltip("Opens multiplexer's packet tracer window");
	ImGui::SameLine(); ImGui::Button("Show PT description");
	ImGui::SetItemTooltip("This is packet tracer feature");

	ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(0.0f, 0.0f), "Searching..");

    /// Table regs
    static ImVec4 bg_color = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    static ImVec4 header_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    static ImVec4 row_bg_color = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    static ImVec4 row_bg_alt_color = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    static ImVec4 text_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    static ImVec4 highlight_color = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);

	if (ImGui::CollapsingHeader("Color Settings")) {
		ImGui::ColorEdit4("Background", (float*)&bg_color);
		ImGui::ColorEdit4("Header", (float*)&header_color);
		ImGui::ColorEdit4("Row BG", (float*)&row_bg_color);
		ImGui::ColorEdit4("Row BG Alt", (float*)&row_bg_alt_color);
		ImGui::ColorEdit4("Text", (float*)&text_color);
		ImGui::ColorEdit4("Highlight", (float*)&highlight_color);
	}

	ImGui::PushStyleColor(ImGuiCol_TableRowBg, row_bg_color);
	ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, row_bg_alt_color);
	ImGui::PushStyleColor(ImGuiCol_Text, text_color);
	ImGui::PushStyleColor(ImGuiCol_Header, header_color);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, highlight_color);
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, highlight_color);

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 6));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));

	const float row_height = ImGui::GetTextLineHeight() + 12.0f;
	const int visible_rows = 8;
	const float table_height = row_height * (visible_rows + 0.5f);

	if (ImGui::BeginTable("RegisterMap", 4,
		ImGuiTableFlags_Borders |
		ImGuiTableFlags_RowBg |
		ImGuiTableFlags_ScrollY,
		ImVec2(0, table_height))) {

		ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 50.0f);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed); // ImGuiTableColumnFlags_WidthStretch
		ImGui::TableSetupColumn("R/W", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed, 130.0f);
		ImGui::TableHeadersRow();

		struct RegisterInfo {
			const char* address;
			const char* name;
			const char* value;
			const char* description;
			bool highlight;
		};

		static RegisterInfo registers[] = {
			{"0x0002", "MUX_TYPES", "R", "Input identify reg\n ID0 - Slot is empty\n ID1 - MUX (2-->6)\n ID2 - MUX 2x(4-->1)\n ID3 - MUX (8-->1)\n ID4 - MUX 4x(2-->1)", false},
			{"0x000A", "CTRL_REG1", "R/W", "Holding Control reg\n    Right slot", true},
			{"0x000B", "CTRL_REG2", "R/W", "Holding Control reg\n   Middle slot", true},
			{"0x000C", "CTRL_REG3", "R/W", "Holding Control reg\n     Left slot", true},
		};

		for (auto& reg : registers) {
			ImGui::TableNextRow();

			if (reg.highlight) {
				ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1,
					ImGui::GetColorU32(highlight_color));
			}

			// Address column
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", reg.address);

			// Name column
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", reg.name);

			// Value column
			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", reg.value);

			// Description column
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%s", reg.description);
		}

		ImGui::EndTable();
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(6);

	ImGui::Text("\n\n");

	static float wrap_width = 250.0f;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
	ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Developed by Max Golubev, JINR\nLaboratory of High Energy Physics");

	draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 255, 255));
	ImGui::PopTextWrapPos();


	ImGui::End();
}
/////////////////////////////////////////////// END MAIN ////////////////////////////////////////////////