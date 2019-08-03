#ifndef __INCLUDE_DEVICES_H
#define __INCLUDE_DEVICES_H
	
class	Mouse
{
public:
	enum	BUTTON {BUTTON_UP, BUTTON_DN};

	void	SetLeftButton(BUTTON in) {eLeftButton = in;}
	BUTTON	GetLeftButton() const {return eLeftButton;}
	void	SetRightButton(BUTTON in) {eRightButton = in;}
	BUTTON	GetRightButton() const {return eRightButton;}
	void	SetWheelDelta(int pos) { iWheelDelta = pos;}
	int		GetWheelDelta() {return iWheelDelta;}
	void	SetWheelActive(bool wa) { wheelActive = wa;}
	bool	GetWheelActive() const { return wheelActive;}

	void	UpdatePosition(int x, int y) {glfXPos = GLfloat(x);  glfYPos = GLfloat(y);}
	void	UpdateXPosition(int x) {glfXPos = GLfloat(x);}
	void	UpdateYPosition(int y) {glfYPos = GLfloat(y);}
	GLfloat	GetXPosition() const {return glfXPos;}
	GLfloat	GetYPosition() const {return glfYPos;}
	bool	IsLeftButtonDown() const {return eLeftButton == BUTTON_DN;}
	bool	IsLeftButtonUp()  const {return eRightButton == BUTTON_UP;}
	bool	IsRightButtonDown() const {return eRightButton == BUTTON_DN;}
	bool	IsRightButtonUp()  const {return eRightButton == BUTTON_UP;}

private:
	BUTTON	eLeftButton;
	BUTTON	eRightButton;

	GLfloat	glfXPos;
	GLfloat	glfYPos;

	int		iWheelDelta;
	bool	wheelActive;
};

class Keyboard
{
public:
	Keyboard() : bSKeyPressed(false), bWKeyPressed(false) {}
	void	SetSKeyPressed(bool b) { bSKeyPressed = b;}
	bool	GetSKeyPressed() const {return bSKeyPressed;}
	void	SetWKeyPressed(bool b) { bWKeyPressed = b;}
	bool	GetWKeyPressed() const {return bWKeyPressed;}
private:
	bool	bSKeyPressed;
	bool	bWKeyPressed;
};
#endif