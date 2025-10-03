#include "Button.h"
#include "Config.h"

void Button::draw() {
	if (is_hovered() == true) { DrawRectangleRec(rec, ScreenC::C[5]); }
	else { DrawRectangleRec(rec, ScreenC::C[2]); }

	Vector2 txt_pos = { rec.x + rec.width / 2 - MeasureText(txt.c_str(), font_sz) / 2, rec.y + 8 };
	DrawText(txt.c_str(), txt_pos.x, txt_pos.y, font_sz, ScreenC::C[0]);
}

bool Button::is_hovered() {
	return CheckCollisionPointRec(GetMousePosition(), rec);
}

bool Button::is_clicked() {
	return CheckCollisionPointRec(GetMousePosition(), rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}