#ifndef MEMENTO_H
#define MEMENTO_H

#include <string>
#include <vector>

class Snapshot {
private:
	std::string board_state;
	int turn_cnt;

	int r[3];
	int c[3];
	int d1;
	int d2;

	friend class GameScreen;

public:
	Snapshot(std::string board_state, int turn_cnt, int r[3], int c[3], int d1, int d2);

};

class Caretaker {
private:
	std::vector<Snapshot> snapshots = {};

public:
	void clear();

	void add_snap(Snapshot snap);
	Snapshot get_snap(int index);
	Snapshot undo();

};

#endif // !MEMENTO_H

