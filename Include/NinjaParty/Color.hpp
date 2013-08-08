#ifndef NINJAPARTY_COLOR_HPP
#define NINJAPARTY_COLOR_HPP

#include <string>

namespace NinjaParty
{
	class Color
	{
	public:
		Color();
		Color(const Color &color) = default;
		Color(float r, float g, float b, float a);
		Color(unsigned int rgba);
		Color(const std::string &rgbaHexString);

		Color& operator=(const Color &color) = default;
		
		Color& operator+=(const Color &c);
		Color& operator-=(const Color &c);
		
		Color& operator*=(const float f);
		Color& operator/=(const float f);
		
		float& R() { return r; }
		float R() const { return r; }
		float& G() { return g; }
		float G() const { return g; }
		float& B() { return b; }
		float B() const { return b; }
		float& A() { return a; }
		float A() const { return a; }

		const static Color White;
		const static Color Black;
		const static Color Red;
		const static Color Green;
		const static Color Blue;
		const static Color Yellow;
		const static Color CornflowerBlue;
		
	protected:
		float r, g, b, a;
	};
	
	const Color operator+(const Color &lhs, const Color &rhs);
	const Color operator-(const Color &lhs, const Color &rhs);
	
	const Color operator*(const float lhs, const Color &rhs);
	const Color operator*(const Color &lhs, const float rhs);
	
	const Color operator/(const Color &lhs, const float rhs);
}

#endif//NINJAPARTY_COLOR_HPP
