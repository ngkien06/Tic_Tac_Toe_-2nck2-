#include "Memento.h"

// ---------------------- <Snapshot> ---------------------

Snapshot::Snapshot(std::string board_state, int turn_cnt, int r[3], int c[3], int d1, int d2) : 
	board_state(board_state), turn_cnt(turn_cnt), d1(d1), d2(d2) {
	for (int i = 0; i < 3; i++) { this->r[i] = r[i]; this->c[i] = c[i]; }
} // "purist memento". not really to my liking, but it work 

// ---------------------- <Caretaker> --------------------

void Caretaker::clear() {
	snapshots.clear();
}

void Caretaker::add_snap(Snapshot snap) {
	snapshots.push_back(snap);
}

Snapshot Caretaker::get_snap(int index) {
	if (index >= snapshots.size() || index < 0) { throw "out of range"; }
	return snapshots[index];
}

Snapshot Caretaker::undo() {
	if (snapshots.size() == 1) { throw "out of range"; }
	snapshots.pop_back();
	return snapshots[snapshots.size() - 1];
}
