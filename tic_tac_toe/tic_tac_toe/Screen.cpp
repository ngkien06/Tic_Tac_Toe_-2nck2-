#include <string>
#include "raylib.h"

#include "Screen.h"
#include "Config.h"
#include "GUI.h"

// --------------- <Screen> ------------------------
Screen::Screen(GUI& gui) : gui_ref(gui) {}

GUI& Screen::get_gui_ref() {
	return gui_ref;
}

// --------------- <MenuScreen> ------------------------

MenuScreen::MenuScreen(GUI& gui) : Screen(gui) {
	button_play.rec = { 0, 5 * ScreenS::ScreenHeight / 8, ScreenS::ScreenWidth / 8, ScreenS::ScreenHeight / 18 };
	button_play.rec.x = ScreenS::ScreenWidth / 2 - button_play.rec.width / 2;
	button_play.txt = "Play";
	button_play.font_sz = 28;
}

void MenuScreen::draw() {
	ClearBackground(ScreenC::C[4]);

	std::string txt = "TIC TAC TOE";
	float w = MeasureText(txt.c_str(), 48);
	DrawText(txt.c_str(), ScreenS::ScreenWidth / 2 - w / 2, ScreenS::ScreenHeight / 8, 48, ScreenC::C[0]);

	button_play.draw();
}

void MenuScreen::update() {
	handle_input();
}

void MenuScreen::handle_input() {
	if (button_play.is_clicked()) { 
		GUI& gui_ref = get_gui_ref();
		std::unique_ptr<Screen> n_scr = std::make_unique<GameScreen>(gui_ref);
		gui_ref.switch_screen(std::move(n_scr));
	}
}

// --------------- <GameScreen> ------------------------

GameScreen::GameScreen(GUI& gui) : Screen(gui) {
	button_back.rec = { ScreenS::ScreenWidth / 70, ScreenS::ScreenHeight / 70, ScreenS::ScreenWidth / 10, ScreenS::ScreenHeight / 22 };
	button_back.txt = "Back";
	button_back.font_sz = 20;

	float cornerX = 9 * ScreenS::ScreenWidth / 160;
	float dis = (6 * ScreenS::ScreenWidth / 10 - 2 * cornerX) / 3; // Divider at 6:4
	float cornerY = 4 * ScreenS::ScreenHeight / 9 - 3 * dis / 2;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) { 
			grid_3x3[i][j].t = ' ';
			grid_3x3[i][j].rec = { cornerX + j * dis, cornerY + i * dis, dis, dis };
		}
	}

	curr_sym = true;
}

void GameScreen::draw() {
	ClearBackground(ScreenC::C[4]);

	button_back.draw();

	draw_grid_3x3();

	//Draw divider at 6:4
	DrawLineEx({ 6 * ScreenS::ScreenWidth / 10, 0 }, { 6 * ScreenS::ScreenWidth / 10, ScreenS::ScreenHeight }, 4, ScreenC::C[3]);


}

void GameScreen::update() {
	handle_input();
}

void GameScreen::handle_input() {
	if (button_back.is_clicked()) {
		GUI& gui_ref = get_gui_ref();
		std::unique_ptr<Screen> n_scr = std::make_unique<MenuScreen>(gui_ref);
		gui_ref.switch_screen(std::move(n_scr));
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (CheckCollisionPointRec(GetMousePosition(), grid_3x3[i][j].rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				//printf("Square (%d,%d): %c\n", i, j, grid_3x3[i][j].t);

				if (grid_3x3[i][j].t == ' ') {
					grid_3x3[i][j].t = curr_sym ? 'O' : 'X';
					curr_sym = !curr_sym;
				}
			}
		}
	}
}

void GameScreen::draw_grid_3x3() {
	float cornerX = 9 * ScreenS::ScreenWidth / 160;
	float dis = (6 * ScreenS::ScreenWidth / 10 - 2 * cornerX) / 3; // Divider at 6:4
	float cornerY = 4 * ScreenS::ScreenHeight / 9 - 3 * dis / 2;	

	DrawLineEx({ cornerX + dis, cornerY }, { cornerX + dis, cornerY + 3 * dis }, 8, ScreenC::C[3]);
	DrawLineEx({ cornerX + 2 * dis, cornerY }, { cornerX + 2 * dis, cornerY + 3 * dis }, 8, ScreenC::C[3]);
	DrawLineEx({ cornerX, cornerY + dis }, { cornerX + 3 * dis, cornerY + dis }, 8, ScreenC::C[3]);
	DrawLineEx({ cornerX, cornerY + 2 * dis }, { cornerX + 3 * dis, cornerY + 2 * dis }, 8, ScreenC::C[3]);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (CheckCollisionPointRec(GetMousePosition(), grid_3x3[i][j].rec)) {
				DrawRectangleRec(grid_3x3[i][j].rec, Fade(ScreenC::C[5], 0.2f));
			}
			
			if (grid_3x3[i][j].t == ' ') { continue; }
			std::string txt = grid_3x3[i][j].t == 'O' ? "O" : "X";
			float w = MeasureText(txt.c_str(), 90);
			DrawText(txt.c_str(), grid_3x3[i][j].rec.x + dis/2 - w/2, grid_3x3[i][j].rec.y + 25, 90, ScreenC::C[0]);
		}
	}

	std::string txt = curr_sym ? "Turn: O" : "Turn: X";
	float w = MeasureText(txt.c_str(), 38);
	DrawText(txt.c_str(), cornerX + 3 * dis / 2 - w / 2, cornerY + 3.5 * dis, 38, ScreenC::C[0]);
}