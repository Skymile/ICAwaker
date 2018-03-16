#include "stdafx.h"

namespace ic
{
	template<typename T> inline
	bool between(T value, T from_inclusive, T to_exclusive)
	{
		return value >= from_inclusive && value < to_exclusive;
	}

	inline Point rect_center(Rect r)
	{
		return Point(r.x + r.width / 2, r.y + r.height / 2);
	}
}