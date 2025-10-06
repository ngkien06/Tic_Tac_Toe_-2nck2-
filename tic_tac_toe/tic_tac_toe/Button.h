#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "raylib.h" 

struct Button {
	Rectangle rec;
	std::string txt;
	int font_sz;
	bool is_shown;

	void draw();
	bool is_hovered();
	bool is_clicked();
};

#endif // !BUTTON_H
