#ifndef SCREEN_H
#define SCREEN_H

#include <memory>
#include <vector>

#include "Button.h"

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

	std::vector<std::pair<int, int>> moves;
	std::string status;
	bool curr_sym;

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
};

#endif // !SCREEN_H
