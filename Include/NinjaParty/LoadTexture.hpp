#ifndef NINJAPARTY_LOADTEXTURE_HPP
#define NINJAPARTY_LOADTEXTURE_HPP

#include <string>

namespace NinjaParty
{
	extern "C"
	{
		int LoadTexture(const std::string &fileName, int &width, int &height);
	}
}

#endif//NINJAPARTY_LOADTEXTURE_HPP
