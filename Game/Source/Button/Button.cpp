
#include "Button.h"
#include "../Application.h"

Button::Button()
{
	pos.Set(0, 0, 0);
	mousePos.Set(0, 0, 0);
	scale.Set(1, 1, 1);
}

Button::~Button()
{
}

void Button::init(Vector3 pos, Vector3 scale)
{
	this->pos = pos;
	this->scale = scale;

	// For debugging only
	Vector3 U(0, 1, 0);
	Vector3 R(1, 0, 0);
	balls[0] = pos + (U * scale.y) + (R * scale.x);
	balls[1] = pos - (U * scale.y) - (R * scale.x);
	balls[2] = pos + (U * scale.y) - (R * scale.x);
	balls[3] = pos - (U * scale.y) + (R * scale.x);
}

bool Button::checkMouse()
{
	updateMouse();

	if ((mousePos.x >= pos.x - scale.x) &&
		(mousePos.x <= pos.x + scale.x) &&
		(mousePos.y >= pos.y - scale.y) &&
		(mousePos.y <= pos.y + scale.y))
		return true;

	return false;
}

float Button::checkMouseLR()
{
	float min = pos.x - scale.x;
	float max = pos.x + scale.x;

	if ((mousePos.x < min) || (mousePos.x > max))
		return -1.0f;

	return (mousePos.x - min) / (max - min);
}

float Button::checkMouseBT()
{
	float min = pos.y - scale.y;
	float max = pos.y + scale.y;

	if ((mousePos.y < min) || (mousePos.y > max))
		return -1.0f;

	return (mousePos.y - min) / (max - min);
}

void Button::updateMouse()
{
	float m_worldHeight = 100.0f;
	float m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	mousePos.x = static_cast<float>((x / w) * m_worldWidth);
	mousePos.y = static_cast<float>(((h - y) / h) * m_worldHeight);
}