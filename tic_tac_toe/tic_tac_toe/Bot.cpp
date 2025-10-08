#include "Bot.h"

Choice::Choice(short mv, short val, short dpt) : move(mv), value(val), depth(dpt) {}

std::string Bot_3x3::has_result(std::string board_state) {
	for (int i = 0; i < 3; i++) {
		if (board_state[3 * i] == board_state[3 * i + 1] && board_state[3 * i] == board_state[3 * i + 2]) {
			if (board_state[3 * i] == 'X') { return "X won"; }
			if (board_state[3 * i] == 'O') { return "O won"; }
		}

		if (board_state[i] == board_state[3 + i] && board_state[i] == board_state[6 + i]) {
			if (board_state[i] == 'X') { return "X won"; }
			if (board_state[i] == 'O') { return "O won"; }
		}
	}
	if ((board_state[0] == board_state[4] && board_state[0] == board_state[8]) ||
		(board_state[2] == board_state[4] && board_state[2] == board_state[6])) {
		if (board_state[4] == 'X') { return "X won"; }
		if (board_state[4] == 'O') { return "O won"; }
	}

	for (int i = 0; i < 9; i++) { if (board_state[i] == ' ') return "Pending"; }
	return "Draw";
}

std::vector<short> Bot_3x3::get_moves(std::string board_state) {
	std::vector<short> res = {};

	for (int i = 0; i < 9; i++) { 
		if (board_state[i] == ' ') { res.push_back(i); }
	}

	return res;
}

Choice Bot_3x3::minimax(std::string board_state, bool curr_sym, short depth) {

	std::string status = has_result(board_state);
	if (status == "X won") { return Choice(9, depth - 10, depth); }
	if (status == "O won") { return Choice(9, 10 - depth, depth); }
	if (status == "Draw")  { return Choice(9, 0, depth); }

	std::vector<short> candiates = get_moves(board_state);
	std::vector<Choice> candiates_choice;
	char t = curr_sym ? 'O' : 'X'; // O: max | X: min
	for (int i = 0; i < candiates.size(); i++) {
		board_state[candiates[i]] = t;

		Choice res = minimax(board_state, !curr_sym, depth + 1);
		res.move = candiates[i];
		candiates_choice.push_back(res);

		board_state[candiates[i]] = ' '; // reset
	}

	Choice max_choice(9, -15, 0), min_choice(9, 15, 0);
	for (int i = 0; i < candiates_choice.size(); i++) {
		if (candiates_choice[i].value > max_choice.value) { max_choice = candiates_choice[i]; }
		if (candiates_choice[i].value < min_choice.value) { min_choice = candiates_choice[i]; }
	}

	/*std::string d = "";
	for (int i = 0; i < depth; i++) { d += "-"; }*/
	if (curr_sym == true) {
		//printf("board state: %s | ", board_state.c_str());
		//printf("max choice(O): mv: %d, val: %d \t| %s\n", max_choice.move, max_choice.value, d.c_str());
		
		return max_choice; 
	}
	else {
		//printf("board state: %s | ", board_state.c_str());
		//printf("min choice(X): mv: %d, val: %d \t| %s\n", min_choice.move, min_choice.value, d.c_str());
		
		return min_choice; 
	}
}