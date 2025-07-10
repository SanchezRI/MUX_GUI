//
// Created by SanchezRIwork a.k.a. campaigner2 on 4/15/25.
//

#include "GuiWindows.h"

///////////////////////////////////////////////// Main window ///////////////////////////////////////////
void GuiWindows::ShowMainWindow(AppState& state) {
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x * 1.0f, viewport->Size.y));

	ImGui::Begin("Commutator", &state.show_commutator_window, state.window_flags);
	ImGui::Text("Here is Commutator window. That controls Multiplexers in commutator unit.																"); ImGui::SameLine();
	if (ImGui::SmallButton("App Info")) {
		ImGui::OpenPopup("App Info Popup");
	}

	if (ImGui::BeginPopupModal("App Info Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Developed by Max Golubev, LHEP JINR\nGithub: https://github.com/SanchezRI/MUX_GUI");
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	static char str0[128] = "192.168.127.254";
	if (ImGui::InputText("IP-addr", str0, IM_ARRAYSIZE(str0))) {
		std::cout << "Callback: New IP-adress: " << str0 << std::endl;
	}

	static int int0 = 502;
	if (ImGui::InputInt("Port       ", &int0)) {
		std::cout << "Callback: New port-number: " << int0 << std::endl;
	}

	static int int1 = 1;
	if (ImGui::InputInt("Unit ID  ", &int1)) {
		std::cout << "Callback: New Unit_ID: " << int1 << std::endl;
	}

	ModbusTcp comm_modbus(str0, int0, 3000);

	// Connection and Popup
	static bool show_connect_popup = false;
	static bool connect_success = false;

	if (ImGui::Button("Connect")) {
		show_connect_popup = true;
		connect_success = comm_modbus.connect();

		if (connect_success) {
			state.comm_connection = true;

			auto input_reg = comm_modbus.readInputRegisters(2, 1, 1);

			auto check_slot = [&](int slot) {
				return comm_modbus.processRegisters(input_reg, 1, 1, false) == std::to_string(slot) ||
					comm_modbus.processRegisters(input_reg, 2, 1, false) == std::to_string(slot) ||
					comm_modbus.processRegisters(input_reg, 3, 1, false) == std::to_string(slot);
				};

			state.mux_slot_id_1 = check_slot(1);
			state.mux_slot_id_2 = check_slot(2);
			state.mux_slot_id_3 = check_slot(3);
			state.mux_slot_id_4 = check_slot(4);

		}
		else {
			state.comm_connection = false;
		}
	} ImGui::SameLine();

	if (show_connect_popup) {
		ImGui::OpenPopup("##ConnectResult");
		show_connect_popup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("##ConnectResult", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (connect_success) {
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Connection Success!");
			if (ImGui::Button("OK", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
		}
		else {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Connection Error!");
			if (ImGui::Button("Retry", ImVec2(120, 0))) {
				show_connect_popup = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}

	// Disconnection
	if (ImGui::Button("Disconnect"))
		ImGui::OpenPopup("Disconnect?"); ImGui::SameLine(); ImGui::SetItemTooltip("Disconnect MUX-device");

	if (ImGui::BeginPopupModal("Disconnect?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Disconnect MUX-device?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			try {
				comm_modbus.disconnect();
				state.comm_connection = false;
			}
			catch (const std::exception& err) {
				state.comm_connection = true;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus(); ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// Status
	ImGui::ColorButton("Commutator Status",
		state.comm_connection ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
		ImGuiColorEditFlags_NoTooltip, ImVec2(25, 25)); ImGui::SameLine();
	ImGui::Text("Commutator connection status: [%s]", state.comm_connection ? "Connected" : "Disconnected");

	if (!state.comm_connection) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Device is disconnected!");
		ImGui::End();
		return;
	} ImGui::SameLine();

	// Polling
	if (ImGui::Checkbox("Polling", &state.comm_polling)) {
		if (state.comm_polling == true) {
			std::cout << "Callback: Turn on poll_command !!!!!" << std::endl;
		}
		else {
			std::cout << "Callback: Turn off poll_command !!!!!" << std::endl;
		}
	}

	/// Table info 1
	static float wrap_width = 285.0f;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MUX types in commutator device", center);
	draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
	ImGui::PopTextWrapPos();

	/// Hardware identification
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 150), ImGuiChildFlags_Borders, window_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTable("MUX Status", 2,
			ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
			ImGuiTableFlags_SizingFixedSame))
		{
			const float column_width = ImGui::GetContentRegionAvail().x * 0.5f - ImGui::GetStyle().CellPadding.x;

			ImGui::TableSetupColumn("MUX", ImGuiTableColumnFlags_WidthFixed, column_width);
			ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, column_width);
			ImGui::TableHeadersRow();

			auto showMuxStatus = [](const char* label, bool isInstalled) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", label);

				ImGui::TableSetColumnIndex(1);
				ImVec2 button_size(20, 20);
				ImGui::ColorButton("##color",
					isInstalled ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1),
					ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder,
					button_size);
				ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
				ImGui::Text("[%s]", isInstalled ? "Installed" : "Not Installed");
				};

			showMuxStatus("MUX_ID = 1", state.mux_slot_id_1);
			showMuxStatus("MUX_ID = 2", state.mux_slot_id_2);
			showMuxStatus("MUX_ID = 3", state.mux_slot_id_3);
			showMuxStatus("MUX_ID = 4", state.mux_slot_id_4);

			ImGui::EndTable();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	/// Table info 2
	static float wrap_width2 = 350.0f;
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width2);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MUX control windows", center);
	draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
	ImGui::PopTextWrapPos();

	/// MUXES tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MUXsTabBar", tab_bar_flags))
	{
		/// MUX slot 1
		if (state.mux_slot_id_1 && ImGui::BeginTabItem("1 x 2 --> 6 pors MUX"))
		{
			ImGui::Text("This is the MUX with ID = 1 tab!");

			auto createPortButtons = [&](const char* portName, int portOffset) {
				char buffer[32];
				ImGui::SeparatorText(portName);
				ImGui::BeginGroup();
				ImGui::NextColumn();

				for (int out = 1; out <= 6; out++) {
					snprintf(buffer, sizeof(buffer), "Set %s x Out%d", portName, out);
					if (ImGui::Button(buffer)) {
						comm_modbus.writeSingleRegister(12, portOffset + out, 1);
						comm_modbus.getPacketLog();
					}
					if (out < 6) ImGui::SameLine();
					ImGui::NextColumn();
				}

				ImGui::EndGroup();
				};

			createPortButtons("In1", 0);
			createPortButtons("In2", 16);
			ImGui::EndTabItem();
		}

		/// MUX slot 2
		if (state.mux_slot_id_2 && ImGui::BeginTabItem("2 x 4 --> 1 ports MUX"))
		{
			ImGui::Text("This is the MUX with ID = 2 tab!");

			struct PortGroup {
				const char* label;
				int base_register;
				int port_number;
			};

			const PortGroup port_groups[] = {
				{"Setters for 1.* x 2.1 ports", 12, 1},
				{"Setters for 1.* x 2.2 ports", 16, 2},
				{"Setters for 1.* x 2.3 ports", 32, 3},
				{"Setters for 1.* x 2.4 ports", 48, 4}
			};

			for (const auto& group : port_groups) {
				ImGui::SeparatorText(group.label);
				ImGui::BeginGroup();
				ImGui::NextColumn();

				for (int i = 1; i <= 4; i++) {
					char button_label[32];
					snprintf(button_label, sizeof(button_label), "Set 1.%d x 2.%d port", i, group.port_number);

					if (ImGui::Button(button_label)) {
						comm_modbus.writeSingleRegister(10, group.base_register + (i - 1), 1);
						comm_modbus.getPacketLog();
					}
					if (i < 4) ImGui::SameLine();
					ImGui::NextColumn();
				}

				ImGui::EndGroup();
			}

			ImGui::EndTabItem();
		}

		/// MUX slot 3
		if (state.mux_slot_id_3 && ImGui::BeginTabItem("1 x 8 --> 1 ports MUX"))
		{
			ImGui::Text("This is the MUX with ID = 3 tab!");

			const struct {
				const char* label;
				int start_port;
				int start_register;
			} port_groups[] = {
				{"Setters for 1 - 4 ports", 1, 0},
				{"Setters for 5 - 8 ports", 5, 4}
			};

			for (const auto& group : port_groups) {
				ImGui::SeparatorText(group.label);
				ImGui::BeginGroup();
				ImGui::NextColumn();

				for (int i = 0; i < 4; i++) {
					const int port_num = group.start_port + i;
					const int reg_num = group.start_register + i;

					char button_label[32];
					snprintf(button_label, sizeof(button_label), "Set %d port", port_num);

					if (ImGui::Button(button_label)) {
						comm_modbus.writeSingleRegister(11, reg_num, 1);
						comm_modbus.getPacketLog();
					}
					if (i < 3) ImGui::SameLine();
					ImGui::NextColumn();
				}

				ImGui::EndGroup();
			}

			ImGui::EndTabItem();
		}

		/// MUX slot 4
#ifdef DEBUG

		if (state.mux_slot_id_4 && ImGui::BeginTabItem("4 x 2 --> 1 pors MUX"))
		{
			ImGui::Text("This is the MUX with ID = 4 tab!");

			ImGui::SeparatorText("Setters for 1 - 2 blocks");
			ImGui::BeginGroup(); ImGui::NextColumn();
			if (ImGui::Button("Set 1.1 port")) {
				comm_modbus.writeSingleRegister(11, 415, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 1.2 port")) {
				comm_modbus.writeSingleRegister(11, 977, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 2.1 port")) {
				comm_modbus.writeSingleRegister(11, 45641, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 2.2 port")) {
				comm_modbus.writeSingleRegister(11, 47893, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::EndGroup();

			ImGui::SeparatorText("Setters for 3 - 4 blocks");
			ImGui::BeginGroup(); ImGui::NextColumn();
			if (ImGui::Button("Set 3.1 port")) {
				comm_modbus.writeSingleRegister(11, 354, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 3.2 port")) {
				comm_modbus.writeSingleRegister(11, 989, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 4.1 port")) {
				comm_modbus.writeSingleRegister(11, 3186, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::SameLine(); ImGui::NextColumn();
			if (ImGui::Button("Set 4.2 port")) {
				comm_modbus.writeSingleRegister(11, 8979, 1);
				comm_modbus.getPacketLog();
			}
			ImGui::EndGroup();

			ImGui::EndTabItem();
		}
#endif // DEBUG
		ImGui::EndTabBar();
	}

	ImGui::End();
}
/////////////////////////////////////////////// END MAIN ////////////////////////////////////////////////