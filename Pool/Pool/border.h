#pragma once
#include "raylib.h"

enum class BorderPosition
{
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT,
	RIGHT,
	LEFT
};

class Border
{
private:
	BorderPosition position;
	Rectangle borderRec;
public:
	Border(BorderPosition position, Rectangle borderRec);
	~Border();
	BorderPosition GetBorderPosition();
	void SetBorderPosition(BorderPosition position);
	Rectangle GetBorderRec();
	void SetBorderRec(Rectangle borderRec);
	void Draw();
};