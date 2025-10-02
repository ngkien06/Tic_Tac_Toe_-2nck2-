#ifndef GUI_H
#define GUI_H

#include <memory>
#include <string>

#include "Screen.h"

class GUI {
private:
	std::unique_ptr<Screen> curr_scr;

public:
	void start();
	void switch_screen(std::unique_ptr<Screen> n_scr);
};

#endif // GUI_H
