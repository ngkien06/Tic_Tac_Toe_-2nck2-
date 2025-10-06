#ifndef SCREEN_H
#define SCREEN_H

#include <memory>
#include <vector>

#include "Button.h"
#include "Memento.h"

class GUI; // forward declaration???

/// --------------

class Screen {
private:
	GUI& gui_ref;

public:
	Screen(GUI& gui);

public:
	GUI& get_gui_ref();

	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handle_input() = 0;

	virtual ~Screen() {}
};

class MenuScreen : public Screen {
private:
	Button button_play;

public:
	MenuScreen(GUI& gui);

public:
	void draw() override;
	void update() override;
	void handle_input() override;
};

class GameScreen : public Screen {
private:
	Button button_back;
	Button button_clear;
	Button button_prev;
	Button button_next;
	Button button_undo;

	struct square_3x3 {
		Rectangle rec;
		char t;
	};
	square_3x3 grid_3x3[3][3];

	struct check_3x3 {
		int r[3] = { 0, 0, 0 };
		int c[3] = { 0, 0, 0 };
		int d1 = 0;
		int d2 = 0;
	};
	check_3x3 c_3x3;

	int turn_cnt;
	std::pair<int, int> lst_mv;
	std::string status;
	bool curr_sym;

	Caretaker caretaker;

public:
	GameScreen(GUI& gui);

public:
	void draw() override;
	void update() override;
	void handle_input() override;

	void print_result();

	void draw_grid_3x3();
	void clear_grid_3x3();
	void check_3x3();

	Snapshot create_snap_3x3();
	void restore_snap_3x3(Snapshot snap);
};

#endif // !SCREEN_H
