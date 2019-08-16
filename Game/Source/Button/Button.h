#ifndef BUTTON_H
#define BUTTON_H

#include "Vector3.h"

class Button
{
private:
	Vector3 mousePos;
	Vector3 pos;
	Vector3 scale;

public:
	Button();
	~Button();

	// For debugging only
	Vector3 balls[4];

	void init(Vector3 pos, Vector3 scale);
	bool checkMouse();

	// Note: LR and BT functions should not be used independently
	// call checkMouse() or updateMouse() first to update the mouse position
	
	float checkMouseLR();	// returns 0.0f to 1.0f, from left to right
	float checkMouseBT();	// returns 0.0f to 1.0f, from bottom to top

	void updateMouse();
};

#endif

// Yours sincerely,
// Ryan