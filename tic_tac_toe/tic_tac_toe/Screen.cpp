#include <string>
#include "raylib.h"

#include "Screen.h"
#include "Config.h"

// --------------- <MenuScreen> ------------------------

void MenuScreen::draw() {
	ClearBackground(ScreenC::C[4]);

	std::string txt = "TIC TAC TOE";
	float w = MeasureText(txt.c_str(), 48);
	DrawText(txt.c_str(), ScreenS::ScreenWidth / 2 - w / 2, ScreenS::ScreenHeight / 8, 48, ScreenC::C[0]);
}

void MenuScreen::update() {
}

void MenuScreen::handle_input() {}