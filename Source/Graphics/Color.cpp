#include <sstream>

#include <NinjaParty/Color.hpp>

namespace NinjaParty
{
	const Color Color::White(1, 1, 1, 1);
	const Color Color::Black(0, 0, 0, 1);
    const Color Color::Gray(0.5f, 0.5f, 0.5f, 1);
	const Color Color::Red(1, 0, 0, 1);
	const Color Color::Green(0, 1, 0, 1);
	const Color Color::Blue(0, 0, 1, 1);
	const Color Color::Yellow(1, 1, 0, 1);
	const Color Color::CornflowerBlue(101/255.0f, 156/255.0f, 239/255.0f, 1);

	Color::Color()
	: r(0), g(0), b(0), a(0)
	{
	}

	Color::Color(float r, float g, float b, float a)
	: r(r), g(g), b(b), a(a)
	{
	}

    Color::Color(unsigned int rgba)
    {
        r = ((rgba >> 24) & 0xff) / (float)0xff;
        g = ((rgba >> 16) & 0xff) / (float)0xff;
        b = ((rgba >> 8) & 0xff) / (float)0xff;
        a = (rgba & 0xff) / (float)0xff;
    }
    
    Color::Color(const std::string &rgbaHexString)
    {
    	unsigned int rgba;
    	std::stringstream converter(rgbaHexString);
    	converter >> std::hex >> rgba;

        r = ((rgba >> 24) & 0xff) / (float)0xff;
        g = ((rgba >> 16) & 0xff) / (float)0xff;
        b = ((rgba >> 8) & 0xff) / (float)0xff;
        a = (rgba & 0xff) / (float)0xff;
    }

	Color& Color::operator+=(const Color &c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}

	Color& Color::operator-=(const Color &c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}

	Color& Color::operator*=(const float f)
	{
		r *= f;
		g *= f;
		b *= f;
		a *= f;
		return *this;
	}

	Color& Color::operator/=(const float f)
	{
		r /= f;
		g /= f;
		b /= f;
		a /= f;
		return *this;
	}

	const Color operator+(const Color &lhs, const Color &rhs)
	{
		return Color(lhs) += rhs;
	}

	const Color operator-(const Color &lhs, const Color &rhs)
	{
		return Color(lhs) -= rhs;
	}

	const Color operator*(const float lhs, const Color &rhs)
	{
		return Color(rhs) *= lhs;
	}

	const Color operator*(const Color &lhs, const float rhs)
	{
		return Color(lhs) *= rhs;
	}

	const Color operator/(const Color &lhs, const float rhs)
	{
		return Color(lhs) * rhs;
	}
}
