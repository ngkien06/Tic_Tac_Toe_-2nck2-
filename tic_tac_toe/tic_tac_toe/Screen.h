#ifndef SCREEN_H
#define SCREEN_H

#include <memory>

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

	struct square_3x3 {
		Rectangle rec;
		char t;
	};
	square_3x3 grid_3x3[3][3];

	bool curr_sym;

public:
	GameScreen(GUI& gui);

public:
	void draw() override;
	void update() override;
	void handle_input() override;

	void draw_grid_3x3();
};

#endif // !SCREEN_H
