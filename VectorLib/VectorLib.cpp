// VectorLib.cpp : Определяет экспортируемые функции для DLL.
//

#include <cmath>
#include "framework.h"
#include "VectorLib.h"


class Vector : public VectorApi
{
public:
	Vector(Number x, Number y) {
		this->x = x;
		this->y = y;
	}

	Number getX()
	{
		return x;
	}

	Number getY()
	{
		return y;
	}

	Number get_Polar_R()
	{
		return Number(sqrt((x * x + y * y).getValue()));
	}

	Number get_Polar_Phi()
	{
		return Number(atan(y.getValue() / x.getValue()));
	}

	void* add(VectorApi* v)
	{
		return new Vector(v->getX() + x, v->getY() + y);
	}

	void destroy()
	{
		delete this;
	}

private:
	Number x;
	Number y;
};

VectorApi* __cdecl CreateInstance(Number x, Number y)
{
	return new Vector(x, y);
}
