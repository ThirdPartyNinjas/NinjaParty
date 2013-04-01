#ifndef NINJAPARTY_RECTANGLE3_HPP
#define NINJAPARTY_RECTANGLE3_HPP

namespace NinjaParty
{
	struct Rectangle
	{
		Rectangle() { }
		
		Rectangle(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height)
		{
		}
		
		int x, y, width, height;
	};
}

#endif//NINJAPARTY_RECTANGLE3_HPP
