#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>

struct Choice {
	short move;
	short value;
	short depth;

	Choice(short mv, short val, short dpt);
};

class Bot_3x3 {
private:

	std::string has_result(std::string board_state); 
	std::vector<short> get_moves(std::string board_state);

public:
	Choice minimax(std::string board_state, bool curr_sym, short depth);

};

#endif // !BOT_H
