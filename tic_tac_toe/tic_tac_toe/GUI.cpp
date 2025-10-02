#include "raylib.h"

#include "GUI.h"
#include "Config.h"

void GUI::start() {
	InitWindow(ScreenS::ScreenWidth, ScreenS::ScreenHeight, "Tic_Tac_Toe");
	SetTargetFPS(60);

	std::unique_ptr<Screen> tmp_ptr = std::make_unique<MenuScreen>(*this);
	curr_scr = std::move(tmp_ptr);

	while (WindowShouldClose() == false) {
		BeginDrawing();
		
		if (curr_scr != nullptr) {
			curr_scr->update();
			curr_scr->draw();
		}

		EndDrawing();
	}

	CloseWindow();
}

void GUI::switch_screen(std::unique_ptr<Screen> n_scr) {
	curr_scr = std::move(n_scr);
}