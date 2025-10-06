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
	button_play.is_shown = true;
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
	button_back.rec = { 60 * ScreenS::ScreenWidth / 70, 65 * ScreenS::ScreenHeight / 70, ScreenS::ScreenWidth / 10, ScreenS::ScreenHeight / 22 };
	button_back.txt = "Back";
	button_back.font_sz = 20;
	button_back.is_shown = true;

	button_clear.rec = { 0, 2 * ScreenS::ScreenHeight / 3, ScreenS::ScreenWidth / 9, ScreenS::ScreenHeight / 21 };
	button_clear.rec.x = 8 * ScreenS::ScreenWidth / 10 - button_clear.rec.width / 2;
	button_clear.txt = "Clear";
	button_clear.font_sz = 24;
	button_clear.is_shown = true;

	button_prev.rec = { 0, 2.5f * ScreenS::ScreenHeight / 5, ScreenS::ScreenWidth / 9, ScreenS::ScreenHeight / 21 };
	button_prev.rec.x = 7.3 * ScreenS::ScreenWidth / 10 - button_prev.rec.width / 2;
	button_prev.txt = "Prev";
	button_prev.font_sz = 24;
	button_prev.is_shown = false;

	button_next.rec = { 0, 2.5f * ScreenS::ScreenHeight / 5, ScreenS::ScreenWidth / 9, ScreenS::ScreenHeight / 21 };
	button_next.rec.x = 8.7 * ScreenS::ScreenWidth / 10 - button_prev.rec.width / 2;
	button_next.txt = "Next";
	button_next.font_sz = 24;
	button_next.is_shown = false;

	button_undo.rec = { 0, 2.5f * ScreenS::ScreenHeight / 5, ScreenS::ScreenWidth / 9, ScreenS::ScreenHeight / 21 };
	button_undo.rec.x = 8 * ScreenS::ScreenWidth / 10 - button_prev.rec.width / 2;
	button_undo.txt = "Undo";
	button_undo.font_sz = 24;
	button_undo.is_shown = true;

	float cornerX = 9 * ScreenS::ScreenWidth / 160;
	float dis = (6 * ScreenS::ScreenWidth / 10 - 2 * cornerX) / 3; // Divider at 6:4
	float cornerY = 4 * ScreenS::ScreenHeight / 9 - 3 * dis / 2;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) { 
			grid_3x3[i][j].t = ' ';
			grid_3x3[i][j].rec = { cornerX + j * dis, cornerY + i * dis, dis, dis };
		}

		c_3x3.r[i] = 0; c_3x3.c[i] = 0;
	}
	c_3x3.d1 = 0; c_3x3.d2 = 0;

	turn_cnt = 0;
	status = "Pending";
	curr_sym = true;

	caretaker.add_snap(create_snap_3x3());
}

void GameScreen::draw() {
	ClearBackground(ScreenC::C[4]);

	button_back.draw();
	button_clear.draw();
	button_prev.draw();
	button_next.draw();
	button_undo.draw();

	draw_grid_3x3();

	//Draw divider at 6:4
	DrawLineEx({ 6 * ScreenS::ScreenWidth / 10, 0 }, { 6 * ScreenS::ScreenWidth / 10, ScreenS::ScreenHeight }, 4, ScreenC::C[3]);
}

void GameScreen::update() {
	handle_input();

	if (status == "O won!" || status == "X won!" || status == "Draw!!!") { // long winded way
		print_result();

		button_next.is_shown = true;
		button_prev.is_shown = true;
		button_undo.is_shown = false;
	}
}

void GameScreen::handle_input() {
	if (button_back.is_clicked()) {
		GUI& gui_ref = get_gui_ref();
		std::unique_ptr<Screen> n_scr = std::make_unique<MenuScreen>(gui_ref);
		gui_ref.switch_screen(std::move(n_scr));
		return;
	}

	if (button_clear.is_clicked()) {
		clear_grid_3x3();
	}

	if (button_prev.is_clicked()) {
		try {
			turn_cnt--;
			restore_snap_3x3(caretaker.get_snap(turn_cnt));
		}

		catch (const char* err_msg) { turn_cnt++; }
	}

	if (button_next.is_clicked()) {
		try {
			turn_cnt++;
			restore_snap_3x3(caretaker.get_snap(turn_cnt));
		}

		catch (const char* err_msg) { turn_cnt--; }
	}

	if (button_undo.is_clicked()) {
		try { restore_snap_3x3(caretaker.undo()); }

		catch (const char* err_msg) { ; }
	}

	if (status != "Pending") { return; }

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (CheckCollisionPointRec(GetMousePosition(), grid_3x3[i][j].rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				//printf("Square (%d,%d): %c\n", i, j, grid_3x3[i][j].t);

				if (grid_3x3[i][j].t == ' ') {
					grid_3x3[i][j].t = curr_sym ? 'O' : 'X';
					lst_mv = { i, j };
					turn_cnt++;
					check_3x3();
					curr_sym = !curr_sym;

					caretaker.add_snap(create_snap_3x3());
				}
			}
		}
	}
}

void GameScreen::print_result() {
	float w = MeasureText(status.c_str(), 60);
	DrawText(status.c_str(), 3 * ScreenS::ScreenWidth / 10 - w / 2, 6 * ScreenS::ScreenWidth / 7, 60, ScreenC::C[0]);
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

	// should this be in draw()?
	std::string txt = curr_sym ? "Turn: O" : "Turn: X";
	float w = MeasureText(txt.c_str(), 38);
	DrawText(txt.c_str(), cornerX + 3 * dis / 2 - w / 2, cornerY + 3.5 * dis, 38, ScreenC::C[0]);
}

void GameScreen::clear_grid_3x3() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) { grid_3x3[i][j].t = ' '; }
	}

	status = "Pending";
	c_3x3 = {
		{0,0,0},
		{0,0,0},
		0,0
	};
	turn_cnt = 0;
	curr_sym = true;
	
	caretaker.clear();
	caretaker.add_snap(create_snap_3x3());

	button_next.is_shown = false;
	button_prev.is_shown = false;
	button_undo.is_shown = true;
}

void GameScreen::check_3x3() {
	int t = (curr_sym) ? 1 : -1;
	int x = lst_mv.first, y = lst_mv.second;

	c_3x3.r[x] += t; c_3x3.c[y] += t;
	if (x == y) { c_3x3.d1 += t; }
	if (x + y == 2) { c_3x3.d2 += t; }

	if (c_3x3.r[x] == 3 || c_3x3.c[y] == 3 || c_3x3.d1 == 3 || c_3x3.d2 == 3) { status = "O won!"; return; }
	if (c_3x3.r[x] == -3 || c_3x3.c[y] == -3 || c_3x3.d1 == -3 || c_3x3.d2 == -3) { status = "X won!"; return; }

	if (turn_cnt == 9) { 
		status = "Draw!!!"; 
		/*for (int i = 0; i < 3; i++) {
			printf("col[%d]: %d, row[%d]: %d\n", i, c_3x3.c[i], i, c_3x3.r[i]);
		}
		printf("diag1: %d, diag2: %d", c_3x3.d1, c_3x3.d2);*/
	} 
}

Snapshot GameScreen::create_snap_3x3() {
	std::string brd_st = "";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			brd_st += grid_3x3[i][j].t;
		}
	}
	//printf("%s:%d\n", brd_st.c_str(), turn_cnt);
	return Snapshot(brd_st, turn_cnt, c_3x3.r, c_3x3.c, c_3x3.d1, c_3x3.d2);
}

void GameScreen::restore_snap_3x3(Snapshot snap) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			grid_3x3[i][j].t = snap.board_state[i * 3 + j];
		}
	}
	turn_cnt = snap.turn_cnt;
	curr_sym = (turn_cnt % 2 == 0) ? true : false; // could just be shorter btw =)

	c_3x3.d1 = snap.d1; c_3x3.d2 = snap.d2;
	for (int i = 0; i < 3; i++) { c_3x3.r[i] = snap.r[i]; c_3x3.c[i] = snap.c[i]; }
}