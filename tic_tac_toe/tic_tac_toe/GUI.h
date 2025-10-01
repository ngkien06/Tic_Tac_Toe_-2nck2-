#ifndef GUI_H
#define GUI_H

#include <memory>

#include "Screen.h"

class GUI {
private:
	std::unique_ptr<Screen> curr_scr;

public:
	void start();
};

#endif // GUI_H
