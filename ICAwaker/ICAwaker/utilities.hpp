#include "stdafx.h"

namespace ic
{
	template<typename T>
	bool between(T value, T from_inclusive, T to_exclusive)
	{
		return value >= from_inclusive && value < to_exclusive;
	}

	Point rect_center(Rect r)
	{
		return Point(r.x + r.width / 2, r.y + r.height / 2);
	}
}