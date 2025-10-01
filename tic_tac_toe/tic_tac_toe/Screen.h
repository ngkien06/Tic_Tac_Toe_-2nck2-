#ifndef SCREEN_H
#define SCREEN_H

class Screen {
public:
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handle_input() = 0;

	virtual ~Screen() {}
};

class MenuScreen : public Screen {
public:
	void draw() override;
	void update() override;
	void handle_input() override;
};

#endif // !SCREEN_H
